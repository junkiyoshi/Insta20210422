#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofNoFill();
	ofSetLineWidth(0.5);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofEnableDepthTest();

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 100, true, true, true);
	this->word = "RAIN";
	int sample_count = 300;
	vector<ofPath> word_path = this->font.getStringAsPoints(word, true, false);
	for (int word_index = 0; word_index < word_path.size(); word_index++) {

		vector<ofPolyline> outline = word_path[word_index].getOutline();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline[outline_index] = outline[outline_index].getResampledBySpacing(1);
			vector<glm::vec3> vertices = outline[outline_index].getVertices();
			for (int i = 0; i < vertices.size(); i++) {

				this->font_location_list.push_back(vertices[i] + glm::vec3(this->font.stringWidth(word) * -0.5, -200, 0));
			}
		}
	}

	ofColor color;
	vector<int> hex_list = { 0x4cc9f0, 0x4895ef, 0x4361ee, 0x3f37c9, 0x3a0ca3, 0x480ca8, 0x560bad, 0x7209b7 };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	if (this->log_list.size() < 1000) {

		for (int i = 0; i < 10; i++) {

			int font_location_index = ofRandom(this->font_location_list.size());
			vector<glm::vec3> log;
			log.push_back(this->font_location_list[font_location_index]);
			this->log_list.push_back(log);
			this->speed_list.push_back(ofRandom(3, 10));
			this->color_list.push_back(this->base_color_list[(int)ofRandom(this->base_color_list.size())]);
		}
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		if (this->log_list[i].back().y > 100) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);

			continue;
		}

		auto next = this->log_list[i].back() + glm::vec3(0, this->speed_list[i], 0);
		this->log_list[i].push_back(next);

		while (this->log_list[i].size() > 15) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);

	ofSetColor(this->base_color_list[0]);
	this->font.drawStringAsShapes(this->word, this->font.stringWidth(this->word) * -0.5, -200);

	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}



	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}