apt-get update

apt-get -y install build-essential git cmake libssl-dev liblua5.3-dev libgl1-mesa-dev wget
#apt-get -y install qtbase5-dev libqt5xmlpatterns5-dev

wget http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run

chmod u+x qt-opensource-linux-x64-5.7.0.run
./qt-opensource-linux-x64-5.7.0.run -platform minimal

mkdir build

cd build

cmake ..

make