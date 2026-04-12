
#ifndef NOKIACHAT_PAN_H
#define NOKIACHAT_PAN_H

/** Nokiachat application panic codes */
enum TNokiachatPanics
	{
	ENokiachatUi = 1
	// add further panics here
	};

inline void Panic(TNokiachatPanics aReason)
	{
	_LIT(applicationName,"Nokiachat");
	User::Panic(applicationName, aReason);
	}

#endif // NOKIACHAT_PAN_H
