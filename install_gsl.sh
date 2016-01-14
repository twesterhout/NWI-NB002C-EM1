#/bin/bash

echo \
'This script is going to download, build and install the GSL (GNU Scientific library).
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
echo "Downloading GSL. This may take some time... "
if ! which wget >/dev/null; then
	echo "You don't have wget installed. Install it first by rinning 'sudo apt-get install wget'."
	exit 1
fi
if ! wget ftp://ftp.gnu.org/gnu/gsl/gsl-2.1.tar.gz; then
	echo "Error, could not complete download."
	exit 1
fi
echo "Done"

prefix=$PWD

echo -n "Unpacking... "
if ! tar -xf gsl-2.1.tar.gz; then
	echo "Error, could not unpack file."
	exit 1
fi
echo "Done"

cd "gsl-2.1"
echo "Beginning installation"
./configure --prefix=$prefix/gsl && make && make install
if [[ ! $? ]]; then
	echo "Error, could not install libraries"
	exit 1
fi
cd ..
echo "Done"

rm -r gsl-2.1
rm gsl-2.1.tar.gz
echo "Successfully installed gsl!"
