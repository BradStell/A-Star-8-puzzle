///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///																							///
///		Author: Brad Stell																	///
///																							///
///		Date: 9/6/2015																		///
///																							///
///		Description:																		///
///			A custom exception class, will display the message passed into the constructor	///
///																							///
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PRIORITYQUEUE_CUSTOMEXCEPTION_H
#define PRIORITYQUEUE_CUSTOMEXCEPTION_H

#include <iostream>
#include <exception>

class CustomException : public std::exception {

private:
	const char* msg;

public:
	CustomException(const char* msg) {
		this->msg = msg;
	}

	virtual const char* what() const throw() {
		return msg;
	}
};

#endif //PRIORITYQUEUE_CUSTOMEXCEPTION_H
