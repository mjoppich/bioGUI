apt-get update
apt-get -y install cmake

mkdir build

cd build

ccmake ..

make