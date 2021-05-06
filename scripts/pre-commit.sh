FILES=$(git diff --cached --name-only | grep -E '.*\.(c|cpp|h|hpp)$')

for FILE in $FILES
do
	echo Formatting $FILE
	clang-format -i $FILE
	git add $FILE
done

echo Formatting Complete

FILES_SRC=$(git diff --cached --name-only | grep -E '.*\.(c|cpp)$')

if [ -n "${FILES_SRC}" ]; then
    echo Analyzing
    cppcheck --error-exitcode=1 --enable=warning,style,performance $FILES_SRC;
        if [ $? -eq 1 ]; then
            echo Analysis Failed
            exit 1
        else
            echo Analysis Complete
        fi
else
    echo No files to analyze
fi

scripts/unit-tests.sh

exit $?