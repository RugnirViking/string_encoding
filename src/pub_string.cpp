#include "ros/ros.h"

#include "std_msgs/String.h"

#include <sstream>
#include <iostream>
#include <fstream>

#include <vector>
#include <bitset>
#include <cmath>

static char charTable[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/',
};
static char pad = '=';

using namespace std;

typedef bitset<24> b64bits;

int main(int argc, char **argv)
{

  ros::init(argc, argv, "string_pub");

  ros::NodeHandle n;

  ros::Publisher encoded_pub = n.advertise<std_msgs::String>("encoded", 1000);

  ros::Rate loop_rate(1);

  int count = 0;//for debug
  while (ros::ok())
  {
	std::cin.ignore();
    //Get the input file, store as vector of chars
	vector<char> data;
	vector<char>::iterator it;

	ifstream in("input", ::ios::binary);
	while (in){
		char input;
		in.get(input);
		if (in) {
			data.push_back(input);
		}
	}

	//Figure out how many padding characters are needed based on file size
	int paddingCharacters = 2-((data.size()-1)%3);

	//Split the chars into sections of 24 bits each
	int localIndex = 0;
	vector<b64bits> bitArray;
	vector<b64bits>::iterator bitIt;
	b64bits bits;

	for (it = data.begin(); it != data.end(); ++it){
		char byte = *it;
		for (int i = 7; i >= 0; --i){
			int power = pow(2, i);
			if (int(byte) >= power){
				byte = byte - power;
				bits.set((7-i)+(8*localIndex));
			}
		}
		if (localIndex == 2){ //Íf index is 2, that means we've completed a cycle of 3 characters, so we can store them
			bitArray.push_back(bits);
			bits.reset();
		}
		localIndex = (localIndex + 1)%3;
	}
	if(bits.to_ulong() != 0){//If the final bitset is empty, don't store it
		bitArray.push_back(bits);
	}

	//Convert the groups of 24 bits into 4 base64 characters
	vector<char> output;
	vector<char>::iterator outIt;

	for (bitIt = bitArray.begin(); bitIt != bitArray.end(); ++bitIt){
		b64bits writeBits = *bitIt;
		for (int i = 0; i < 4; ++i){
			bitset<6> sixBits;
			for (int j = 0; j < 6; ++j){
				if(writeBits.test(i*6+j)){
					sixBits.set(5-j);
				}
			}
			output.push_back(charTable[sixBits.to_ulong()]);
		}
	}

	//Make the 0-bits at the end into correct padding characters
	for (int i = 0; i < paddingCharacters; ++i){
		output.pop_back();
	}
	for(int i = 0; i < paddingCharacters; ++i){
		output.push_back('=');
	}

	//Convert output to string
	string outputString( output.begin(), output.end() );
    //some program
    //std::ifstream readfile("foo.jpg");
    //std::string file_contents ((std::istreambuf_iterator<char>(readfile)), std::istreambuf_iterator<char>() );
    //readfile.close();
    //encryption data
    //some program 
    
    std_msgs::String msg;

    //std::stringstream ss;
    //ss << file_contents;//this is where the string goes // count is for de bug
    msg.data = outputString;

    ROS_INFO("%s", msg.data.c_str());

    encoded_pub.publish(msg);

    ros::spinOnce();

    //loop_rate.sleep();

    //count ++;//for debug
  }


  return 0;
}

