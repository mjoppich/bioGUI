apt-get update

apt-get -y install build-essential git cmake libssl-dev liblua5.3-dev libgl1-mesa-dev wget libfontconfig1 libdbus-1-dev
apt-get -y install libglu1-mesa-dev

#apt-get -y install qtbase5-dev libqt5xmlpatterns5-dev

wget http://download.qt.io/official_releases/qt/5.11/5.11.1/qt-opensource-linux-x64-5.11.1.run

chmod u+x qt-opensource-linux-x64-5.11.1.run
./qt-opensource-linux-x64-5.11.1.run --verbose -platform minimal --script ./silent_qt_install.qs

export PATH=/usr/local/qt/5.11.1/gcc_64/bin/:$PATH

mkdir build

cd build

cmake ..

make
