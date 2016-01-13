#/bin/bash


echo \
'This script is going to download, build and install the needed Boost libraries.
You okay with that? (yes/no)'

read answer
while [[ $answer != "yes" && $answer != "no" ]]; do
	echo "please, answer 'yes' or 'no'"
	read answer
done


if [[ $answer == no ]]; then
	exit 1
fi

# Download Boost
echo -n "Downloading Boost. This may take some time... "
if ! which wget >/dev/null; then
	echo "You don't have wget installed. Install it first by rinning 'sudo apt-get install wget'."
	exit 1
fi
if ! wget http://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.bz2/download; then
	echo "Error, could not complete download."
	exit 1
fi
echo "Done"

prefix=$PWD

echo -n "Unpacking... "
if ! tar -xf download; then
	echo "Error, could not unpack file."
	exit 1
fi
echo "Done"


cd "boost_1_60_0"
echo "Beginning installation"
./bootstrap.sh --with-libraries=filesystem,iostreams,system --prefix=$prefix/boost >/dev/null && ./b2 install


if [[ ! $? ]]; then
	echo "Error, could not install libraries"
	exit 1
fi
echo "Done"

echo "-L$prefix/boost/lib"
echo "-I$prefix/boost/include"

rm -r boost_1_60
rm download
