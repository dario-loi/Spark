#pragma once
#include <vector>
#include "../../Spark/SparkConfig.h"
#include <cstdint>

class VBO
{
private:
	unsigned int RenderID;
	std::vector<float> verts;
	std::vector<spark::SparkInstanceData> instances{spark::INITIAL_INSTANCES};

	size_t instanceCapacity{spark::INITIAL_INSTANCES};
	size_t instanceBaseOffset;

public:

	 explicit VBO(std::vector<float>&& vertices) noexcept;
	~VBO() noexcept;
	

	void Bind() const;
	void Unbind() const;

	void setData(std::vector<float>&& newArr);

	[[nodiscard]] unsigned int getRenderID() const { return RenderID; }

	[[nodiscard]] constexpr size_t getSize() const { return verts.size(); }
	[[nodiscard]] std::vector<float> const& getArray() const { return verts; }

	[[nodiscard]] std::vector<spark::SparkInstanceData> const& getInstanceData() const
	{
		return instances;
	}

	[[nodiscard]] constexpr size_t getInstSize() const { return instances.size(); }
	[[nodiscard]] constexpr size_t getInstCapacity() const { return instances.capacity(); }

	[[nodiscard]] constexpr size_t getTotalSize() const { return (instances.size() + verts.size()); }

	void setInstanceSubData(std::vector<spark::SparkInstanceData> const& data, size_t offset) noexcept;
	void setInstanceData(std::vector<spark::SparkInstanceData> const& data) noexcept;

	[[nodiscard]] size_t addInstance(spark::SparkInstanceData& inst) noexcept;


	[[nodiscard]] std::vector<float> const& getData() const { return verts; }

};

