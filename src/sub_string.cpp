
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <fstream>
#include <string>
#include <iostream>
#include <bitset>
#include <cmath>

//Namespaces and types
using namespace std;
typedef bitset<24> b64bits;

//The 64 characters used - 8x8 normal characters, and a padding character
static char charTable[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/','='
};

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());//for debug prints to console
  

	int padds=0;
	//Array indexed by the characters of the charTable - allows reversal of the table
	int ia[127]; 
	for (int i = 0; i < 128; ++i){
		ia[i] = 0;
	}
	for (int i = 0; i < 64; ++i){
		ia[charTable[i]] = i;
		//cout << charTable[i] << " maps to " << ia[charTable[i]] << '\n';
	}

	//Store input as string
	string inputString = msg->data.c_str();
	string::iterator inIt;

	vector<int> data;
	vector<int>::iterator it;

	//Convert to list of integers
	for(inIt = inputString.begin(); inIt != inputString.end(); ++inIt){
		if(ia[*inIt] == 0){
			padds++;
		}
		//cout << ia[*inIt] << '\n'; 
		data.push_back( ia[*inIt] );
	}

	//Split the chars into sections of 24 bits each
	int localIndex = 0;
	vector<b64bits> bitArray;
	vector<b64bits>::iterator bitIt;
	b64bits bits;

	for (it = data.begin(); it != data.end(); ++it){
		int byte = *it;
		if (byte == 65){
			cout << "Hi!";
			byte = 0;
		}
		for (int i = 5; i >= 0; --i){
			int power = pow(2, i);
			if (byte >= power){
				byte = byte - power;
				bits.set((5-i)+(6*localIndex));
			}
		}
		if (localIndex == 3){ //Íf index is 3, that means we've completed a cycle of 4 characters, so we can store them
			//cout << bits/*.to_string()*/ << '\n';

			bitArray.push_back(bits);
			bits.reset();
		}
		localIndex = (localIndex + 1)%4;
	}

	vector<char> output;
	vector<char>::iterator outIt;

	for (bitIt = bitArray.begin(); bitIt != bitArray.end(); ++bitIt){
		b64bits writeBits = *bitIt;
		for (int i = 0; i < 3; ++i){
			bitset<8> eightBits;
			for (int j = 0; j < 8; ++j){
				if(writeBits.test(i*8+j)){
					eightBits.set(7-j);
				}
			}
			output.push_back((char) eightBits.to_ulong());
		}
	}

	//Convert output to string
	string outputString( output.begin(), output.end()-padds);





//some program
  //msg->data.c_str() decryption
//    std::ifstream readfile("foo.jpg");
//    std::string file_contents ((std::istreambuf_iterator<char>(readfile)),std::istreambuf_iterator<char>() );
//    readfile.close();

    std::ofstream writefile("outut");
    writefile << outputString;
    writefile.close();
  //some program 
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "string_sub");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("encoded", 1000, chatterCallback);

  ros::spin();

  return 0;
}
