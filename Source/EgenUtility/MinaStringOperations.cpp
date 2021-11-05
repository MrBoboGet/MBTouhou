#include<EgenUtility\MinaStringOperations.h>
std::vector<std::string> Split(std::string UrsprungsStringen, std::string SplitStringen)
{
	std::vector<std::string> Result = std::vector<std::string>(0);
	int StartCharacter = 0;
	size_t NuvarandeSplit = 0;
	NuvarandeSplit = UrsprungsStringen.find(SplitStringen, StartCharacter);
	while (NuvarandeSplit != UrsprungsStringen.npos)
	{
		Result.push_back(UrsprungsStringen.substr(StartCharacter, NuvarandeSplit - StartCharacter));
		StartCharacter = NuvarandeSplit + SplitStringen.size();
		NuvarandeSplit = UrsprungsStringen.find(SplitStringen, StartCharacter);
	}
	Result.push_back(UrsprungsStringen.substr(StartCharacter));
	return(Result);
}