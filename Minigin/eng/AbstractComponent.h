#pragma once

namespace eng {

/// <summary>
/// An abstract base class for Components
/// </summary>
class AbstractComponent {
public: 
	virtual ~AbstractComponent() = default;

	virtual std::string Serialize() = 0;
	virtual void Deserialize(std::string serializationString) = 0;

	virtual void Start()		{};

	virtual void Update()		{};
	virtual void LateUpdate()	{};
	virtual void FixedUpdate()	{};

	virtual void Render()		{};

	virtual void OnDestroy()	{};
};

}