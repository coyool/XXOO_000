##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=app
ConfigurationName      :=Debug
WorkspacePath          := "F:\backup\2013-2-21\jzq\USER\prj\asbj_app"
ProjectPath            := "F:\backup\2013-2-21\jzq\USER\prj\asbj_app\app"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Administrator
Date                   :=2013-2-21
CodeLitePath           :="D:\Program Files\CodeLite"
LinkerName             :=gcc
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="app.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=windres
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := gcc
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)


##
## User defined environment variables
##
CodeLiteDir:=D:\Program Files\CodeLite
UNIT_TEST_PP_SRC_DIR:=D:\UnitTest++-1.3
Objects0=$(IntermediateDirectory)/USER_main$(ObjectSuffix) $(IntermediateDirectory)/USER_stm32f10x_it$(ObjectSuffix) 

Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/USER_main$(ObjectSuffix): ../../../main.c $(IntermediateDirectory)/USER_main$(DependSuffix)
	$(CC) $(SourceSwitch) "F:/backup/2013-2-21/jzq/USER/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/USER_main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/USER_main$(DependSuffix): ../../../main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/USER_main$(ObjectSuffix) -MF$(IntermediateDirectory)/USER_main$(DependSuffix) -MM "../../../main.c"

$(IntermediateDirectory)/USER_main$(PreprocessSuffix): ../../../main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/USER_main$(PreprocessSuffix) "../../../main.c"

$(IntermediateDirectory)/USER_stm32f10x_it$(ObjectSuffix): ../../../stm32f10x_it.c $(IntermediateDirectory)/USER_stm32f10x_it$(DependSuffix)
	$(CC) $(SourceSwitch) "F:/backup/2013-2-21/jzq/USER/stm32f10x_it.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/USER_stm32f10x_it$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/USER_stm32f10x_it$(DependSuffix): ../../../stm32f10x_it.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/USER_stm32f10x_it$(ObjectSuffix) -MF$(IntermediateDirectory)/USER_stm32f10x_it$(DependSuffix) -MM "../../../stm32f10x_it.c"

$(IntermediateDirectory)/USER_stm32f10x_it$(PreprocessSuffix): ../../../stm32f10x_it.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/USER_stm32f10x_it$(PreprocessSuffix) "../../../stm32f10x_it.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/USER_main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/USER_main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/USER_main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/USER_stm32f10x_it$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/USER_stm32f10x_it$(DependSuffix)
	$(RM) $(IntermediateDirectory)/USER_stm32f10x_it$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) $(OutputFile).exe
	$(RM) "../.build-debug/app"


