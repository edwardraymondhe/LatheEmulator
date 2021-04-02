#ifndef CYLINDER_H
#define CYLINDER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>



using namespace std;

class Knife {
public:
	glm::vec3 position;
	float width;
	//Model model;

	//Knife(glm::vec3 pos, float w, Model m) : position(pos), width(w), model(m){}
	Knife(glm::vec3 pos, float w) : position(pos), width(w) { }
};

struct Slice {
	// position A
	float posA;
	// position B
	float posB;
	// radius of the slice
	float rad;

	Slice() {}
	Slice(float a, float b, float r) : posA(a), posB(b), rad(r) {}
};


class Cylinder {
public:
	vector<Slice> slices;
	//float length;
	int slice_count;
	float slice_width;
	float slice_rad;

	Cylinder(int len, float w, float r)
	{
		slice_width = w;
		slice_rad = r;
		//length = len * w;
		float start = 0.0f;
		for (int i = 0; i < len; i++)
		{
			slices.push_back(Slice(start, start + w, r));
			start += w;
		}

		slice_count = slices.size();
	}

	Slice SliceGenerate(Knife knife)
	{
		// Calculates positions and radius for new Slice
		float mouseX, mouseY;
		float posA = mouseX - knife.width;
		float posB = mouseX + knife.width;
		return Slice(posA, posB, knife.width);
	}

	bool CylinderCut(Knife knife, bool bounded, vector<float> &bezierArray, int currBezierIdx) {

		int start_pos = -1;
		int end_pos = -1;

		bool reach_fore_of_cylinder = false;
		bool reach_end_of_cylinder = true;
		bool cut_no_match = true;

		bool result = false;

		int a, b, c;
		for(int i = 0; i < slice_count; i++)
		{
			Slice slice = slices.at(i);

			//float knife_left = knife.position.x - knife.width;


			if (knife.position.x > (*slices.rbegin()).posB)
			{
				// cout << "Larger than end" << endl;
				return result;
			}

			// cout << slice_count << " " << knife.position.x << " " << slice.posA << " " << slice.posB << endl;
			if (knife.position.x - knife.width <= slice.posB)
			{
				if (knife.position.x - knife.width > slice.posA)
				{
					start_pos = i;
				}
				else
				{
					break;
				}
			}
		}

		if (start_pos == -1)
			return result;

		end_pos = start_pos;
		for(int i = start_pos; i < slice_count; i++)
		{
			Slice slice = slices.at(i);

			if (knife.position.x + knife.width<= slice.posB)
			{
				if (knife.position.x + knife.width > slice.posA)
				{
					end_pos = i;
				}
				else {
					break;
				}
			}
		}
		if (end_pos != -1)
		{
			for(int i = start_pos; i <= end_pos; i++)
			{
				// TODO: Change later, for more realistic knifing
				if (knife.position.y - 0.5f >= 0 && knife.position.y - 0.5f <= slices[i].rad)
				{
					if (bounded == true)
					{
						if (slices[i].rad >= bezierArray[i])
							slices[i].rad -= 0.003f;
					}
					else
					{
						slices[i].rad -= 0.003f;
					}
					result = true;
				}
			}
		}

		return result;
		//		If found
		//			delete all the slices fall between posA, posB
		//			generate two new slices	that're composed of posA, posB
		//		If not found
		//			If posA && posB fall inside one slice
		//				delete the slice
		//				generate two new slices that're composed of posA, posB
		//			If posA && posB fall outside of all slices
		//				decrease the slices rad that're bigger than knife.position.y

		// Sort slices[]
	}
};

#endif