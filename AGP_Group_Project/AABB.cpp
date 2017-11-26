		
		//This commented out code was me trying to allow for orientation of the bounding box
		//but it's probably not worth doing

		/*glm::mat4 rotation = glm::translate(rotation, positions[0]);
		rotation = glm::rotate((rotation), float(45 * DEG_TO_RADIAN), glm::vec3(1, 0, 0));
		rotation = glm::scale(rotation, scale);
		*/

		//this->min = positions[0];
		//this->max = positions[0];
		/*this->min = glm::vec3(1);
		this->max = glm::vec3(1);

		this->min = glm::vec3(  glm::vec4(min,0) * rotation );
		this->max = glm::vec3(  glm::vec4( max,0) * rotation );*/

		//this->min -=  scale * glm::vec3(positions.size());
		//this->max += scale * glm::vec3(positions.size());
	
