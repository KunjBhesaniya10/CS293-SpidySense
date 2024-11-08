#include <vector>
#include<iostream>
#include<algorithm>
#include "plagiarism_checker.cpp"
static bool cmp(Match &a, Match &b)
{
    return a.end < b.end;
}
int eraseOverlapIntervals(std::vector<Match> &intervals)
{
    sort(intervals.begin(), intervals.end(), cmp);
    int lastEnd = intervals[0].end;
    int matchID=intervals[0].MatchingTo;
    int cnt = 1;
    int cnt1=1;
    for (int i = 1; i < intervals.size(); i++)
    {
        if (intervals[i].start > lastEnd)
        {
            cnt++;
            if(intervals[i].MatchingTo!=matchID) {
                cnt1++;
            }
            lastEnd = intervals[i][1];
        }
    }
    //cnt is number of non overlapping intervals
    if(cnt1!=1&&cnt1>=20) std::cout<<"hurrah"<<std::endl; //Patch work
    else std::cout<<"bach gaya "<<std::endl;
}