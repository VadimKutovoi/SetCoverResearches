mkdir build
cd build

cmake ..
make

./runTests
res=$?

if [ $res -eq 0 ]; then
    cd ..
    rm -r build
    echo "runTests.sh success!"
else
    echo "runTests.sh failed!"
    echo "keeping build folder"
fi
