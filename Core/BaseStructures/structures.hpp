#include <string>
#include <vector>

namespace resp_struct{
    struct SChallengeInfo
    {
        std::string Uid;
        std::string Name;
        std::string FileName;
        std::string Author;
        std::string Environnement;
        std::string Mood;
        int BronzeTime;
        int SilverTime;
        int GoldTime;
        int AuthorTime;
        int CopperPrice;
        bool LapRace;
        int NbLaps;
        int NbCheckpoints;
    };

    struct SPlayerRanking {
        std::string Login;
        std::string NickName;
        int PlayerId;
        int Rank;
        int BestTime;
        std::vector<int> BestCheckpoints;
        int Score;
        int NbrLapsFinished;
        double LadderScore;
    };

    struct SPlayerInfo {
        std::string Login;
        std::string NickName;
        int PlayerId;
        int TeamId;
        int SpectatorStatus;
        int LadderRanking;
        int Flags;
    };

    enum VoteStateNames{
        NewVote, VoteCancelled, VotePassed, VoteFailed
    };
}