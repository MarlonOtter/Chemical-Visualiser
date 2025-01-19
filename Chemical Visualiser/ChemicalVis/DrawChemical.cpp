#include "DrawChemical.h"


void DrawChemical::Draw(std::vector<Chemical> chemicals, Model& atomModel, Model& bondModel, Camera camera, Shader& atomShader, Shader& bondShader)
{
	AtomDrawData allAtomDrawData;
	Transforms allBondTransforms;
	for (int i = 0; i < chemicals.size(); i++)
	{
		AtomDrawData atomDrawData = calcAtoms(chemicals[i]);
		merge(atomDrawData, allAtomDrawData);

		Transforms bondTransforms = calcBonds(chemicals[i]);
		merge(bondTransforms, allBondTransforms);
	}
	//add the colours
	VAO& atomVAO = atomModel.meshes[0].VAO;
	VBO atomVBO(allAtomDrawData.colours);
	atomVAO.Bind();
	atomVBO.Bind();
	//does this need  to be called every frame?
	//it is the easiest way i can think of and its not particually intensive now that i fixed memory leak
	atomVAO.LinkAttrib(atomVBO, 4, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);
	glVertexAttribDivisor(4, 1);
	atomVBO.Unbind();

	//draw the model
	atomModel.DrawInstanced(atomShader, camera, allAtomDrawData.transforms.pos, allAtomDrawData.transforms.rotat, allAtomDrawData.transforms.scale);
	bondModel.DrawInstanced(bondShader, camera, allBondTransforms.pos, allBondTransforms.rotat, allBondTransforms.scale);

	atomVBO.Delete();
}

AtomDrawData DrawChemical::calcAtoms(Chemical& chemical)
{
	Transforms transforms;
	std::vector<glm::vec3> colours;

	for (int i = 0; i < chemical.atoms.size(); i++)
	{
		Atom atom = chemical.atoms[i];
		// check if there is an atom which has no element associated with it
		if (chemical.atoms[i].element == 0) {
			std::cout << "ERROR: Atom with no element" << std::endl;
			continue;
		}
		//set the position of the atom object to the correct location

		transforms.pos.push_back(atom.pos);

		//the scale is multiplied by a set value. This may be able to be changed in settings once i implement that
		transforms.scale.push_back(glm::vec3(globalClass::atomScale) * std::min(1.0f, static_cast<float>(atom.element) / 2.0f));

		//get the colour of the atom to be displayed
		//atomObj.shader->Activate();
		colours.push_back(atom.colour);
		//glUniform3f(glGetUniformLocation(atomShader.ID, "atomColour"), colr.r, colr.g, colr.b);
	}
	return AtomDrawData{ transforms, colours };
}

Transforms DrawChemical::calcBonds(Chemical& chemical)
{
	Transforms transforms;

	for (int i = 0; i < chemical.bonds.size(); i++)
	{
		Bond bond = chemical.bonds[i];
		for (int j = 0; j < bond.count; j++)
		{
			//break; // no need to do the loop as it does nothing atm
			
			float bondOffset = 0;
			if (bond.count > 1) 
			{
				float dist = globalClass::bondSeperationDist;
				bondOffset = dist * j - (dist * (bond.count - 1) / 2);
			}

			//this only works for the 2D chemical structure atm
			glm::vec3 pointA = chemical.atoms[bond.atomA - 1].pos + glm::vec3(0.0f, bondOffset, 0.0f);
			glm::vec3 pointB = chemical.atoms[bond.atomB - 1].pos + glm::vec3(0.0f, bondOffset, 0.0f);

			glm::vec3 midPoint = (pointA + pointB) / 2.0f;
			transforms.pos.push_back(midPoint);

			//calculate the rotation needed to point at the atoms
			glm::vec3 dir = glm::normalize(pointB - pointA);
			glm::vec3 initialAxis = glm::vec3(0, 1, 0);
			glm::quat rotation = glm::rotation(initialAxis, dir);

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