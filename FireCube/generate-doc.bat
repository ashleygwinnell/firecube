doxygen Doxyfile
cd Documentation
cd html
svn add *.*
svn propset svn:mime-type "text/html" *.html
cd search
svn add *.*
svn propset svn:mime-type "text/html" *.html
cd..
cd..
cd..