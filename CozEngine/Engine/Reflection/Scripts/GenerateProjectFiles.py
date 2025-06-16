import os
import re
import xml.etree.ElementTree as ET

# Currently assumes strict formatting of c++ code.
# 1. REFLECT_CLASS is used on the line immediately above the class declaration line.
# 2. Clean formatting of lines. I.e. Reflection relevant stuff such as class/ property declarations
#    are on their own line.
# 3. Curly brackets have their own line. I.e. Allman Style.

REFLECT_CLASS_TAG = "REFL_CLASS"
REFLECT_PROPERTY_TAG = "REFL_PROP"

ExcludedPaths = ["Engine\\Include", "Engine\\lib", "Engine\\Content", "Engine\\Assets",
                 "Game\\Assets", "Game\\Content"]

GeneratedFilesPath = "..\\..\\GeneratedFiles\\"

def IsExcludedPath(Path):
    for ExcludedPath in ExcludedPaths:
        if Path.lower().endswith(ExcludedPath.lower()):
            return True
    return False

class GeneratedFile:
    def __init__(Self, Name, Path):
        Self.Name = Name
        Self.Path = Path
        Self.ReflectedClasses = []

class ReflectedClass:
    def __init__(Self, Name, ParentName):
        Self.Name = Name
        Self.Properties = []
        Self.AdditionalIncludes = set()

        if not ParentName:
            Self.ParentName = ""
        else:
            Self.ParentName = ParentName

class ReflectedProperty:
    def __init__(Self, Name, Type, Tags):
        Self.Name = Name
        Self.Type = Type
        Self.Tags = Tags

    def __str__(self):
        return f"{self.Name}, {self.Type}"

PropertyTypeToIncludeMap = {
    "glm::vec3" : "Misc/GlmSerialization.h"
}

TemplatePropertyTypeToIncludeMap = {
    "LResourceHandle" : "ResourceManagement/ResourceHandleHelper.h"
}

ReflectedClassSet = set()

GeneratedFiles = []

VarNameRegex = r"\w{1}\w*"
# \s*(\w{1}\w*(?:::\w{1}\w*)*(?:<\w{1}\w*>)*)\s+(\w{1}\w*)
VarIdentifierRegex = r"\s*(" + VarNameRegex + r"(?:::" + VarNameRegex + r")*(?:<" + VarNameRegex + r">)*)\s+(" + VarNameRegex + r")"

def ProcessDirectory(Directory):
    for Filename in os.listdir(Directory):
        FullPath = os.path.join(Directory, Filename)
        if os.path.isdir(FullPath):
            if not IsExcludedPath(FullPath):
                ProcessDirectory(FullPath)
        elif os.path.isfile(FullPath):
            _, Extension = os.path.splitext(Filename)
            if Extension == '.h':
                ProcessHeader(FullPath)

CurrentFilePath = ""
def ProcessHeader(HeaderFilePath):
    global CurrentFilePath
    with open(HeaderFilePath, "r", encoding="utf-8") as F:
        Lines = []
        ReflectTagLineIndex = []
        for LineIndex, Line in enumerate(F, 1):
            Lines.append(Line)
            if Line.startswith(REFLECT_CLASS_TAG):
                ReflectTagLineIndex.append(LineIndex)

        if len(ReflectTagLineIndex) > 0:
            HeaderName = os.path.splitext(os.path.basename(HeaderFilePath))[0]
            NormalizedPath = HeaderFilePath.replace("\\", "/")
            PathMatch = re.search(f"(?:/Engine/|/Game/)(.*)/{HeaderName}.h", NormalizedPath)
            GeneratedFiles.append(GeneratedFile(HeaderName, PathMatch.group(1)))

        for LineIndex in ReflectTagLineIndex:
            CurrentFilePath = HeaderFilePath
            ProcessReflectedClass(LineIndex, Lines)

def PrintInvalidReflectedClassTagUsage(LineNumber):
    print(f"Invalid {REFLECT_CLASS_TAG} usage in " + CurrentFilePath + " on line " + str(LineNumber))

def PrintInvalidReflectedPropertyTagUsage(LineNumber):
    print(f"Invalid {REFLECT_PROPERTY_TAG} usage in " + CurrentFilePath + " on line " + str(LineNumber))

def ProcessReflectedClass(ReflectTagLineIndex, Lines):
    ClassNameMatch = re.search(r"(?:class|struct)\s*(.\S+)\s*(?:: public (\S+))?", Lines[ReflectTagLineIndex])
    ClassName = ""
    try:
        ClassName = ClassNameMatch.group(1)
    except:
        PrintInvalidReflectedClassTagUsage(ReflectTagLineIndex + 1)
        print("Failed to find class name match")
        exit(1)

    ReflectedClassSet.add(ClassName)

    ParentClassName = ClassNameMatch.group(2)

    CurrentReflectedClasses = GeneratedFiles[len(GeneratedFiles) - 1].ReflectedClasses
    CurrentReflectedClasses.append(ReflectedClass(ClassName, ParentClassName))

    CurrentLineIndex = ReflectTagLineIndex + 2
    for Line in Lines[ReflectTagLineIndex + 2 : len(Lines)]:
        if Line.startswith('}'):
            break

        PropertyTagMatch = re.search(f"\\s*({REFLECT_PROPERTY_TAG})\\((.*)\\)", Line)
        if PropertyTagMatch and PropertyTagMatch.group(1):
            ProcessReflectedProperty(Lines, CurrentLineIndex, CurrentReflectedClasses[len(CurrentReflectedClasses) - 1], PropertyTagMatch.group(2).split(','))
        CurrentLineIndex += 1

def ProcessReflectedProperty(Lines, ReflectTagLineIndex, ReflectedClass, Tags):
    PropertyMatch = re.search(VarIdentifierRegex, Lines[ReflectTagLineIndex + 1])

    TypeStartIndex = 0
    TypeStopIndex = 0
    IdentifierStartIndex = 0
    IdentifierStopIndex = 0

    OpenScopes = 0

    for i in range(0, len(Lines[ReflectTagLineIndex + 1])):
        Char = Lines[ReflectTagLineIndex + 1][i]
        if not TypeStopIndex:
            if not Char == ' ' and not TypeStartIndex:
                TypeStartIndex = i
            elif Char == '<':
                OpenScopes += 1
            elif Char == '>':
                OpenScopes -= 1
            elif Char == ' ' and not OpenScopes:
                TypeStopIndex = i - 1

        if TypeStopIndex and not Char == ' ' and not IdentifierStartIndex:
            IdentifierStartIndex = i
        elif IdentifierStartIndex and (Char == ' ' or Char == ';') and not IdentifierStopIndex:
            IdentifierStopIndex = i - 1
            break

    TypeName = Lines[ReflectTagLineIndex + 1][TypeStartIndex : TypeStopIndex + 1]
    PropertyName = Lines[ReflectTagLineIndex + 1][IdentifierStartIndex : IdentifierStopIndex + 1]

    ReflectedClass.Properties.append(ReflectedProperty(PropertyName, TypeName, Tags))

    if TypeName in PropertyTypeToIncludeMap:
        ReflectedClass.AdditionalIncludes.add(PropertyTypeToIncludeMap[TypeName])
    else:
        for TemplateType in TemplatePropertyTypeToIncludeMap:
            if TemplateType in TypeName:
                ReflectedClass.AdditionalIncludes.add(TemplatePropertyTypeToIncludeMap[TemplateType])

EngineDir = '..\\..\\..\\Engine'
ProjectDir = '..\\..\\..\\Game'

ProcessDirectory(EngineDir)
ProcessDirectory(ProjectDir)

VcxprojPath = "..\\..\\..\\CozEngine.vcxproj"
Tree = ET.parse(VcxprojPath)
Root = Tree.getroot()
ns = {'msb': 'http://schemas.microsoft.com/developer/msbuild/2003'}

CppItemGroup = None
for ItemGroup in Root.findall(".//msb:ItemGroup", ns):
    if ItemGroup.find("msb:ClCompile", ns) is not None:
        CppItemGroup = ItemGroup
        break

if CppItemGroup is None:
    print("Failed to find ClCompile ItemGroup in .vcxproj")
    exit(1)

# Clear generated files from VS project files
for ClCompile in CppItemGroup.findall("msb:ClCompile", ns):
    FileName = ClCompile.get("Include")
    if FileName.lower().startswith("Engine\\GeneratedFiles\\".lower()):
        CppItemGroup.remove(ClCompile)

for GenFile in GeneratedFiles:
    with open(GeneratedFilesPath + GenFile.Name + "Gen.cpp", "w") as File:

        File.write(f"""\
#include \"{GenFile.Path}/{GenFile.Name}.h\"

#include \"Editor/ImGuiPropertyDrawHelpers.h\"
#include \"Reflection/Class.h\"
#include \"Reflection/ClassRegister.h\"
#include \"Reflection/ClassUtilities.h\"
#include \"Reflection/Property.h\"
""")

        ClassSpecificHeaders = """"""

        for Class in GenFile.ReflectedClasses:
            ReflectedProperties = """"""
            DeserializeProperties = """"""
            SerializeProperties = """    Json[\"Type\"] = \""""+ Class.Name + "\";\n"""
            DrawProperties = """"""

            for i in range(len(Class.Properties)):
                Property = Class.Properties[i]
                ReflectedProperties += "            {\"" + Property.Name + "\", \"" + Property.Type + "\", MEMBER_OFFSET(" + Class.Name + ", " + Property.Name + ")}"
                DeserializeProperties += "    Object." + Property.Name + " = Json[\"" + Property.Name + "\"];\n"
                SerializeProperties += "    Json[\"" + Property.Name + "\"] = Object." + Property.Name + ";\n"
                
                DrawnProperty = False
                if "Visible" in Property.Tags:
                    DrawProperties += "                " + Property.Type + "* " + Property.Name + "Ptr = reinterpret_cast<" + Property.Type + "*>(Address + offsetof(" + Class.Name + ", " + Property.Name + "));\n"
                    DrawProperties += "                LImGuiPropertyDrawHelpers::DrawProperty(\"" + Property.Name + "\", *" + Property.Name + "Ptr);"
                    DrawnProperty = True

                if i < len(Class.Properties) - 1:
                    ReflectedProperties += ",\n"
                    if DrawnProperty:
                        DrawProperties += "\n\n"

            for Include in Class.AdditionalIncludes:
                ClassSpecificHeaders += "#include \"" + Include + "\"\n"

            ToJson = """"""
            FromJson = """"""
            ParentName = ""
            if Class.ParentName and Class.ParentName in ReflectedClassSet:
                ParentName = Class.ParentName
                ToJson = "    const " + ParentName + "& Parent = Object;\n    to_json(Json, Parent);"
                FromJson = "    " + ParentName + "& Parent = Object;\n    from_json(Json, Parent);"

            File.write(f"""\
{ClassSpecificHeaders}

LClass* {Class.Name}::Class = {Class.Name}::StaticClass();

LClass* {Class.Name}::StaticClass()
{{
    if(!{Class.Name}::Class)
    {{
        {Class.Name}::Class = new LClass();
        assert({Class.Name}::Class);

        std::vector<LProperty> Properties
        {{
{ReflectedProperties}
        }};

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {{
{DrawProperties}
            }};

        
        std::function<void*()> CreateObjectFunc = []()
            {{
                return new {Class.Name}();
            }};

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {{
                const {Class.Name}* Object = reinterpret_cast<const {Class.Name}*>(Address);
                to_json(Json, *Object);
            }};

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {{
                {Class.Name}* Object = reinterpret_cast<{Class.Name}*>(Address);
                from_json(Json, *Object);
            }};

        LClassUtilities::RegisterStaticClassProperties({Class.Name}::Class,
                                                        Properties,
                                                        sizeof({Class.Name}),
                                                        alignof({Class.Name}),
                                                        \"{Class.Name}\",
                                                        \"{ParentName}\",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc);
        
        LClassRegister::RegisterObj(\"{Class.Name}\", {Class.Name}::Class);
    }}
    return {Class.Name}::Class;
}}

void from_json(const nlohmann::json& Json, {Class.Name}& Object)
{{
{FromJson}
{DeserializeProperties}
}}

void to_json(nlohmann::json& Json, const {Class.Name}& Object)
{{
{ToJson}
{SerializeProperties}
}}
""")
            
    ET.SubElement(CppItemGroup, "ClCompile", Include=f"Engine\\GeneratedFiles\\{GenFile.Name}Gen.cpp")

# Required to keep write to .vcxproj clean
ET.register_namespace("", "http://schemas.microsoft.com/developer/msbuild/2003")

Tree.write(VcxprojPath)
