#include <String>
#include <irrKlang.h>
using namespace std;

class Sound {

	public:
		Sound();
		Sound(string filename_);
		Sound(string filename, bool loopSound);

		void setFile(string filename);
		void setMode(bool loop);
		void play();
		void drop();

	private:
		irrklang::ISoundEngine* engine;
		string path = "";
		bool loopSound = false;
		bool playing;

};
