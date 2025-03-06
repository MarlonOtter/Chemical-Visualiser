#include "DrawChemical.h"

int DrawChemical::prevChemicalCount = 0;
std::vector<glm::vec3> DrawChemical::atomColours;

drawData DrawChemical::chemicalDrawData;

void DrawChemical::Draw(std::vector<Chemical> chemicals, Model& atomModel, Model& bondModel, Camera camera, Shader& atomShader, Shader& bondShader, int format)
{
	assert((format == 1 || format == 2) && "Format should be either 2:3D or 1:2D to be valid");

	int newSize = static_cast<int>(chemicals.size());
	
	if (chemicals.size() == 0)
	{
		chemicalDrawData.clear();
		return;
	}

	if (prevChemicalCount != newSize || !chemicalDrawData.done2D || !chemicalDrawData.done3D)
	{
		if (prevChemicalCount != newSize)
		{
			prevChemicalCount = newSize;
			chemicalDrawData.clear();
		}

		if (format == 1)
		{
			chemicalDrawData.done2D = true;
		}
		else if (format == 2)
		{
			chemicalDrawData.done3D = true;
		}

		//std::cout << "New Data\n";

		Transforms atomTransforms;
		Transforms bondTransforms;

		atomColours.clear();

		for (int i = 0; i < chemicals.size(); i++)
		{
			glm::vec3 chemOffset = glm::vec3(0.0f, 0.0f, i * globalClass::chemicalSeperationDist);
			if (format == Chemical::_2D) chemOffset = glm::vec3(i * globalClass::chemicalSeperationDist, 0, 0);

			AtomDrawData nAtomDrawData = calcAtoms(chemicals[i], i, format, chemOffset);
			merge(nAtomDrawData.transforms, atomTransforms);
			merge(nAtomDrawData.colours, atomColours);

			Transforms nBondTransforms = calcBonds(chemicals[i], i, format, chemOffset);
			merge(nBondTransforms, bondTransforms);
		}
		std::vector<glm::mat4> atomMat4s = Model::convertToMatrices(atomTransforms.pos, atomTransforms.rotat, atomTransforms.scale);
		std::vector<glm::mat4> bondMat4s = Model::convertToMatrices(bondTransforms.pos, bondTransforms.rotat, bondTransforms.scale);

		chemicalDrawData[2 * (format - 1)] = atomMat4s;
		chemicalDrawData[2 * (format - 1) + 1] = bondMat4s;

	}
	//add the colours
	VAO& atomVAO = atomModel.meshes[0].VAO;
	VBO atomVBO(atomColours);

	atomVAO.Bind();
	atomVBO.Bind();
	//does this need  to be called every frame?
	//it is the easiest way i can think of and its not particually intensive now that i fixed memory leak
	atomVAO.LinkAttrib(atomVBO, 4, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);
	glVertexAttribDivisor(4, 1);
	atomVBO.Unbind();

	

	//draw the model
	atomModel.DrawInstanced(atomShader, camera, chemicalDrawData[2*(format-1)]);
	bondModel.DrawInstanced(bondShader, camera, chemicalDrawData[2*(format-1)+1]);

	atomVBO.Delete();
}

AtomDrawData DrawChemical::calcAtoms(Chemical& chemical, int index, int format, glm::vec3 chemicalOffset)
{
	Transforms transforms;
	std::vector<glm::vec3> colours;

	for (int i = 0; i < chemical.atoms.size(); i++)
	{
		Atom atom = chemical.atoms[i];
		// Z is offset so that the model always overlays the bond model
		glm::vec3 atomPos = glm::vec3(atom.pos2D, 1.0f);
		if (format == 2)
		{
			atomPos = atom.pos3D;
		}

		// check if there is an atom which has no element associated with it
		if (chemical.atoms[i].element == 0) {
			std::cout << "ERROR: Atom with no element" << std::endl;
			continue;
		}
		//set the position of the atom object to the correct location
		
		transforms.pos.push_back(atomPos + chemicalOffset);

		//the scale is multiplied by a set value. This may be able to be changed in settings once i implement that
		transforms.scale.push_back(glm::vec3(globalClass::atomScale) * std::min(1.0f, static_cast<float>(atom.element) / 2.0f));

		// add the colour of the atom to the colours list
		colours.push_back(atom.colour);
	}
	return AtomDrawData{ transforms, colours };
}

Transforms DrawChemical::calcBonds(Chemical& chemical, int index, int format, glm::vec3 chemicalOffset)
{
	Transforms transforms;

	for (int i = 0; i < chemical.bonds.size(); i++)
	{
		Bond bond = chemical.bonds[i];
		for (int j = 0; j < bond.count; j++)
		{

			
			glm::vec3 pointA = glm::vec3(chemical.atoms[bond.atomA - 1].pos2D, 0);
			glm::vec3 pointB = glm::vec3(chemical.atoms[bond.atomB - 1].pos2D, 0);

			if (format == 2)
			{
				pointA = chemical.atoms[bond.atomA - 1].pos3D;
				pointB = chemical.atoms[bond.atomB - 1].pos3D;
			}

			glm::vec3 midPoint = ((pointA + pointB) / 2.0f);

			//calculate the rotation needed to point at the atoms
			glm::vec3 dir = glm::normalize(pointB - pointA);
			glm::vec3 initialAxis = glm::vec3(0, 1, 0);
			glm::quat rotation = glm::rotation(initialAxis, dir);

			glm::vec3 bondOffset(0,0,0);
			if (bond.count > 1) 
			{
				// calculate the vector that is perpendicular to the line AB
				bondOffset = (glm::normalize(glm::cross(pointA, pointB)));
				//rotate around 0,0,0 in the axis parallel to the bond
				bondOffset = rotateAround(bondOffset, 90.0f, glm::vec3(0), glm::normalize(pointB - pointA));
				// multiply the vector to get the offset of the bond to the centre
				float dist = globalClass::bondSeperationDist;
				bondOffset *= dist * j - (dist * (bond.count - 1) / 2);
			}
			//add any offsets and add to the list
			midPoint += chemicalOffset + bondOffset;
			transforms.pos.push_back(midPoint);

			
			// add rotations and scales to the list
			transforms.rotat.push_back(rotation);
			transforms.scale.push_back(glm::vec3(globalClass::bondRadius, globalClass::bondLengthMultiplier * glm::distance(pointA, pointB), globalClass::bondRadius));
		}
	}
	return transforms;
}


int DrawChemical::merge(AtomDrawData& src, AtomDrawData& dst)
{
	merge(src.transforms, dst.transforms);
	merge(src.colours, dst.colours);
	return 0;
}

int DrawChemical::merge(Transforms& src, Transforms& dst)
{
	merge(src.pos, dst.pos);
	merge(src.scale, dst.scale);

	dst.rotat.insert(dst.rotat.end(), src.rotat.begin(), src.rotat.end());
	return 0;
}

int DrawChemical::merge(std::vector<glm::vec3>& src, std::vector<glm::vec3>& dst)
{
	dst.insert(dst.end(), src.begin(), src.end());
	return 0;
}


glm::vec3 DrawChemical::rotateAround(glm::vec3 pos, float degrees, glm::vec3 center, glm::vec3 axis)
{
	float angle = glm::radians(degrees);
	//translate pos to origin relative to center
	glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -center);
	//Apply rotation
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
	//translate back to position around center
	glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), center);
	//combine transformations
	glm::mat4 transformation = translateBack * rotation * translateToOrigin;
	//apply to object
	return glm::vec3(transformation * glm::vec4(pos, 1.0f));
}