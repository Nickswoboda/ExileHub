for FILE in $(git diff --cached --name-only | grep -E '.*\.(c|cpp|h|hpp)$')
do
	clang-format -i $FILE
	echo Formatting $FILE
	git add $FILE
done