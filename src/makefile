WORK := $(shell  pwd)
APP := ORION
CXX := g++
CXXFLAGS := -c -O3 -Wall -fmessage-length=0 -std=c++0x
#CXXFLAGS := -c -O0 -g -Wall -fmessage-length=0 -std=c++0x
#PREPROC :=  -DLINUXOS  -DiCdebugstatement -DiCdebugvalidation -DiCdebugqryl2 -DiCfastpool -DHAVE_NETINET_IN_H -DHAVE_INTTYPES_H -DiCdebugqryl2 -DiCtcmalloc
#PREPROC :=  -DsCpipename='"ORIONPIPE_"' -DLINUXOS  -DiCfastpool -DHAVE_NETINET_IN_H -DHAVE_INTTYPES_H -DiCtcmalloc -DiCoo2sortedindex
PREPROC :=  -DsCpipename='"ORIONPIPE_"' -DAPPLEOS  -DiCfastpool -DHAVE_NETINET_IN_H -DHAVE_INTTYPES_H -DiCtcmalloc -DiCoo2sortedindex
#PREPROC :=  -DLINUXOS  -DiCfastpool -DHAVE_NETINET_IN_H -DHAVE_INTTYPES_H -DiCtcmalloc 
INCLUDES := -I"/usr/include" -I"$(WORK)/Thrft" -I"$(WORK)/Extrn/md5" -I"$(WORK)/Engne" -I"$(WORK)/Extrn" -I"$(WORK)/Proto" -I"$(WORK)/Hlprs" -I/opt/protobuf/include -I/opt/glog/include -I/opt/gtest/include -I/opt/gflags/include -I/opt/snappy/include -I/opt/libevent/include -I/opt/thrift/include/thrift -I/opt/boost/include -I/opt/gperftools/include 
LIBS := -L/usr/local/lib -L/opt/libevent/lib -L/opt/thrift/lib -L/opt/snappy/lib -L/opt/gtest -L/opt/gflags/lib -L/opt/glog/lib -L/opt/protobuf/lib -L/opt/gperftools/lib
REF := -lthrift -lthriftnb -levent -lsnappy -lre2 -lgtest -ltcmalloc -lpthread -lprotobuf -lgflags -lglog
DIRS := Thrft Proto Hlprs Engne Extrn Unitt Extrn/md5 . 
BUILDIR := Build
DATADIR := /opt/orion
EXCLUDES := $(shell cat exclude.mk)
FILES := $(foreach dir,$(DIRS),$(wildcard $(dir)/*.cc))
FILES += $(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp))
OBJECT := $(patsubst %,$(addprefix $(BUILDIR)/,%.o), $(filter-out $(EXCLUDES),$(FILES)))

all: $(APP)
	@echo 'qui $(files)' 

$(APP): $(OBJECT)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CXX) $(PREPROC) $(INCLUDES) $(LIBS) -o $(BUILDIR)/$(APP) $(OBJECT) $(REF)
	@echo 'Finished building target: $@'
	#@cp $(BUILDIR)/$(APP) $(DATADIR)
	@echo 'cp $(BUILDIR)/$(APP) to $(DATADIR)'
	@echo ' '


$(BUILDIR)/%.cpp.o: %.cpp 
	@echo $(EXCLUDES)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo 'FILE $(@F)'
	@mkdir -p $(@D)
	$(CXX) $(PREPROC) $(INCLUDES) $(CXXFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(BUILDIR)/%.cc.o: %.cc 
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo 'FILE $(@F)'
	@mkdir -p $(@D)
	$(CXX) $(PREPROC) $(INCLUDES) $(CXXFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean:
	@rm -r $(BUILDIR)/*
