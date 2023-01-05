#include "KRMath\KRMath.h"
#include <string>

int main()
{
	//KRM::FVector3 testVec{};
	KRM::FVector2 testVec2{};
	KRM::FVector2 testVec1{};
	testVec2.Cross(testVec1);

	KRM::FVector3 test3Vec1{};
	KRM::FVector3 test3Vec2{};

	test3Vec1.Cross(test3Vec2);
	//KRM::FVector3 test1 = testVec.Cross(testVec);
	//KRM::FVector3 test2 = KRM::Cross(testVec, test1);
	return 0;
}