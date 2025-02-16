#pragma once

namespace _namespace_ {

class ClassHeader final {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	ClassHeader()	= default;
	~ClassHeader()	= default;

	ClassHeader				(const ClassHeader&)	= default;
	ClassHeader& operator=	(const ClassHeader&)	= default;

	ClassHeader				(const ClassHeader&&)	= default;
	ClassHeader& operator=	(const ClassHeader&&)	= default;


/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------

}; // !ClassHeader

} // !_namespace_