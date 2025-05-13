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
    def __init__(Self, Name):
        Self.Name = Name
        Self.Properties = []

class ReflectedProperty:
    def __init__(Self, Name, Type):
        Self.Name = Name
        Self.Type = Type

    def __str__(self):
        return f"{self.Name}, {self.Type}"

GeneratedFiles = []

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
    ClassNameMatch = re.search(r"class\s*(.\S*)\s*", Lines[ReflectTagLineIndex])
    ClassName = ""
    try:
        ClassName = ClassNameMatch.group(1)
    except:
        PrintInvalidReflectedClassTagUsage(ReflectTagLineIndex + 1)
        print("Failed to find class name match")
        exit(1)

    CurrentReflectedClasses = GeneratedFiles[len(GeneratedFiles) - 1].ReflectedClasses
    CurrentReflectedClasses.append(ReflectedClass(ClassName))

    CurrentLineIndex = ReflectTagLineIndex + 2
    for Line in Lines[ReflectTagLineIndex + 2 : len(Lines)]:
        if Line.startswith('}'):
            break

        PropertyTagMatch = re.search(f"\\s*{REFLECT_PROPERTY_TAG}", Line)
        if PropertyTagMatch:
            ProcessReflectedProperty(Lines, CurrentLineIndex, CurrentReflectedClasses[len(CurrentReflectedClasses) - 1])
        CurrentLineIndex += 1

def ProcessReflectedProperty(Lines, ReflectTagLineIndex, ReflectedClass):
    PropertyMatch = re.search(r"\s*(\w[a-zA-Z0-9]+)\s+(\w[a-zA-Z0-9]+)", Lines[ReflectTagLineIndex + 1])

    TypeName = ""
    try:
        TypeName = PropertyMatch.group(1)
    except:
        PrintInvalidReflectedPropertyTagUsage(ReflectTagLineIndex + 1)
        print("Failed to find property type match")
        exit(1)

    PropertyName = ""
    try:
        PropertyName = PropertyMatch.group(2)
    except:
        PrintInvalidReflectedPropertyTagUsage(ReflectTagLineIndex + 1)
        print("Failed to find property name match")
        exit(1)

    ReflectedClass.Properties.append(ReflectedProperty(PropertyName, TypeName))

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

#include \"Reflection/Class.h\"
#include \"Reflection/ClassRegister.h\"
#include \"Reflection/ClassUtilities.h\"
#include \"Reflection/Property.h\"

""")
        
        for Class in GenFile.ReflectedClasses:
            ReflectedProperties = """"""
            for i in range(len(Class.Properties)):
                Property = Class.Properties[i]
                ReflectedProperties += "            {\"" + Property.Name + "\", \"" + Property.Type + "\", MEMBER_OFFSET(" + Class.Name + ", " + Property.Name + ")}"
                if i < len(Class.Properties) - 1:
                    ReflectedProperties += ",\n"

            File.write(f"""\
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

        LClassUtilities::RegisterStaticClassProperties({Class.Name}::Class, Properties);
        LClassRegister::RegisterObj(\"{Class.Name}\", {Class.Name}::Class);
    }}
    return {Class.Name}::Class;
}}          
""")
            
    ET.SubElement(CppItemGroup, "ClCompile", Include=f"Engine\\GeneratedFiles\\{GenFile.Name}Gen.cpp")

# Required to keep write to .vcxproj clean
ET.register_namespace("", "http://schemas.microsoft.com/developer/msbuild/2003")

Tree.write(VcxprojPath)
