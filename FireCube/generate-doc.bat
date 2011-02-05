doxygen Doxyfile
cd Documentation
cd html
svn add *.* --auto-props
cd search
svn add *.* --auto-props
REM svn propset svn:mime-type "text/html" *.html
REM cd projects
REM cd FireCube
REM svn add *.html
REM svn propset svn:mime-type "text/html" *.html
cd..
cd..
cd..