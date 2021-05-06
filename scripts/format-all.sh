cd $(dirname "$0")

for FILE in $(find ../src | grep -E '.*\.(c|cpp|h|hpp)$')
do
	clang-format -i $FILE
	echo Formatted $FILE
done

for FILE in $(find ../tests | grep -E '.*\.(c|cpp|h|hpp)$')
do
	clang-format -i $FILE
	echo Formatted $FILE
done
