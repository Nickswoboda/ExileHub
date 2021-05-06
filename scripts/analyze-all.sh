cd $(dirname "$0")

cppcheck --error-exitcode=1 --enable=warning,style,performance ../src ../tests;