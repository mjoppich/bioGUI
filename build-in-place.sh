apt-get update

apt-get -y install build-essential git cmake libssl-dev liblua5.3-dev
apt-get -y install qtbase5-dev libqt5xmlpatterns5-dev

mkdir build

cd build

cmake ..

make