#pragma once
#include "Buffers/EBO.h"
#include "Buffers/VAO.h"
#include "Buffers/VBO.h"
#include <string>
#include <cstdint>
#include "boost/uuid/uuid.hpp"

namespace spark
{
	class Model
	{
	private:

		const boost::uuids::uuid UUID;

		VAO m_Vao;
		VBO m_Vbo;
		EBO m_Ebo;

		std::string name;

	public:
		Model(boost::uuids::uuid UUID_, std::string const& name_, std::vector<float>&& vertices, std::vector<unsigned int>&& indices);
		~Model() = default;
		Model(Model const&) = default;
		Model(Model&&) = default;
		Model& operator=(const Model&) = default;

		void Bind() const;
		void Unbind() const;

		void ModelInit() const;

		VAO& getVAO();
		EBO& getEBO();

		[[nodiscard]] std::vector<unsigned int> const& getIndexReference() const;
		[[nodiscard]] std::vector<float> const& getBufferReference() const;

		[[nodiscard]] std::string getName() const;
		[[nodiscard]] const boost::uuids::uuid getUUID() const;

		void setName(std::string const& name_);


		size_t getIndexSize() const;
		size_t getBufferSize() const;

		[[nodiscard]] VBO const& getVBO() const;

	};

}	