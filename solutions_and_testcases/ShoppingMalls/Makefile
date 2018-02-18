
LATEX=pdflatex

TestCasesDir := TestCases

JavaSourceFiles := $(wildcard Solutions/Place.java)
JavaSourceFiles += $(wildcard Solutions/*.java)
JavaClassFiles  := $(JavaSourceFiles:%.java=%.class)

SourceFiles := $(wildcard Solutions/*.cpp)
SourceFiles += $(wildcard Solutions/*.cc)
ObjFiles  := $(SourceFiles:%.cpp=%.o)
ObjFiles  += $(SourceFiles:%.cc=%.o)
ExeFiles  := $(SourceFiles:%.cpp=%.exe)
ExeFiles  += $(SourceFiles:%.cc=%.exe)

all: ShoppingMalls.pdf java_classes ${JavaClassFiles} ${ExeFiles}

clean:
	rm -f  *.exe Solutions/*.exe
	rm -rf *.exe.dSYM
	rm -f  *.class classes/*.class
	rm -rf *.log *.aux *.out *.snm *.nav *.toc *.vrb

%.exe: %.cpp
	g++ -g -Wall -O3 -o $@  $<

%.exe: %.cc
	g++ -g -Wall -O3 -o $@  $<

java_classes:
	mkdir -p classes
	javac -d classes -cp classes  Solutions/*.java

%.class: %.java
	mkdir -p classes
	javac -d classes -cp classes  $<

%.pdf: %.tex
	@$(LATEX) $< ;  \
	while ( grep "Rerun to get cross-references" $(<:%.tex=%.log) >/dev/null ); do \
		$(LATEX) $< ; \
	done; 

