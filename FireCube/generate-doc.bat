cppdoc_cmd.exe -title=FireCube -classdir=projects -module="cppdoc-standard" -extensions="c,cpp,cxx,cc,h,hpp,hxx" -languages="c=cpp,cc=cpp,cpp=cpp,cs=csharp,cxx=cpp,h=cpp,hpp=cpp,hxx=cpp,java=java" -overwrite "..\FireCube" #Debug #Release #.svn #glee.c #glee.h #privateFont.h ".\Documentation\index.html"
cd Documentation
cd projects
cd FireCube
svn add *.html
svn propset svn:mime-type "text/html" *.html
cd..
cd..
cd..