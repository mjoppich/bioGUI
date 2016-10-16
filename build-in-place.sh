apt-get update

apt-get -y build-essential
apt-get -y install qtbase5-dev
apt-get -y install cmake

mkdir build

cd build

ccmake ..

make