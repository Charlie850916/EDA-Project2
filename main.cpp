#include <bits/stdc++.h>

#define MAXCELL 1500000

using namespace std;

typedef long long lint;

struct cell
{
    lint id, area, setID;
    vector<lint> adjNet;
};

struct net
{
    lint connect;
    vector<lint> adj;
    pair<lint,lint> distribute;
};

fstream fin, fout;
lint maxArea, totalCell, totalNet;

vector<cell> cellArr;
vector<net> netArr;

void init()
{
    fstream farea, fnet;
    string str;
    fin >> str;
    farea.open(str, ios::in);
    fin >> str;
    fnet.open(str, ios::in);
    fin >> maxArea;
    fin.close();
    farea >> totalCell;
    for(lint i=0 ; i<totalCell ; ++i)
    {
        cell in;
        farea >> in.id >> in.area;
        cellArr.push_back(in);
    }
    farea.close();
    fnet >> totalNet;
    for(lint i=0 ; i<totalNet ; ++i)
    {
        net in;
        fnet >> in.connect;
        in.distribute.first = in.distribute.second = 0;
        for(lint j=0 ; j<in.connect ; ++j)
        {
            lint k;
            fnet >> k;
            in.adj.push_back(k);
            cellArr[k].adjNet.push_back(i);
        }
        netArr.push_back(in);
    }
    fnet.close();
}

lint areaA, areaB;

void initPartion()
{
    areaA = areaB = 0;
    for(lint i=0 ; i<cellArr.size() ; ++i)
    {
        if(areaA <= areaB)
        {
            areaA += cellArr[i].area;
            cellArr[i].setID = 0;
            for(lint id : cellArr[i].adjNet)
                netArr[id].distribute.first++;
        }
        else
        {
            areaB += cellArr[i].area;
            cellArr[i].setID = 1;
            for(lint id : cellArr[i].adjNet)
                netArr[id].distribute.second++;
        }
    }
}

lint gain[MAXCELL];
bool fix[MAXCELL];
void updateGain()
{
    lint FS[totalCell], TE[totalCell];
    memset(FS, 0, totalCell*sizeof(lint));
    memset(TE, 0, totalCell*sizeof(lint));
    for(lint i=0 ; i<netArr.size() ; ++i)
    {
        if(netArr[i].distribute.first==0)
        {
            for(lint id : netArr[i].adj)
            {
                TE[id]++;
            }
        }
        else if(netArr[i].distribute.first==1)
        {
            for(lint id : netArr[i].adj)
            {
                if(cellArr[id].setID==0)
                {
                    FS[id]++;
                    break;
                }
            }
        }
        if(netArr[i].distribute.second==0)
        {
            for(lint id : netArr[i].adj)
            {
                TE[id]++;
            }
        }
        else if(netArr[i].distribute.second==1)
        {
            for(lint id : netArr[i].adj)
            {
                if(cellArr[id].setID==1)
                {
                    FS[id]++;
                    break;
                }
            }
        }
    }
    for(lint i=0 ; i<totalCell ; ++i)
        gain[i] = FS[i] - TE[i];
}

lint chooseCell()
{
    lint choose = -1;
    for(lint i=0 ; i<cellArr.size() ; ++i)
    {
        if(!fix[i])
        {
            if(cellArr[i].setID==0)
                if(areaB + cellArr[i].area > maxArea)
                    continue;
            if(cellArr[i].setID==1)
                if(areaA + cellArr[i].area > maxArea)
                    continue;
            if(gain[i] > 0)
            {
                if(choose==-1)
                {
                    choose = i;
                }
                else if(gain[i] > gain[choose])
                {
                    choose = i;
                }
            }
        }
    }
    return choose;
}

void FM()
{
    memset(gain, 0, totalCell*sizeof(lint));
    memset(fix, false, totalCell*sizeof(bool));
    while(1)
    {
        updateGain();
        lint choose = chooseCell();
        if(choose==-1)
            return;
        fix[choose] = true;
        if(cellArr[choose].setID==0)
        {
            areaA -= cellArr[choose].area;
            areaB += cellArr[choose].area;
            for(lint id : cellArr[choose].adjNet)
            {
                netArr[id].distribute.first--;
                netArr[id].distribute.second++;
            }
            cellArr[choose].setID = 1;
        }
        else
        {
            areaA += cellArr[choose].area;
            areaB -= cellArr[choose].area;
            for(lint id : cellArr[choose].adjNet)
            {
                netArr[id].distribute.first++;
                netArr[id].distribute.second--;
            }
            cellArr[choose].setID = 0;
        }
    }
}

void solve()
{
    initPartion();
    lint cost = INT_MAX;
    lint curCost = 0;
    for(lint i=0 ; i<netArr.size() ; ++i)
        if(netArr[i].distribute.first != 0 && netArr[i].distribute.second != 0)
            curCost++;
    while(cost > curCost)
    {
        cost = curCost;
        FM();
        curCost = 0;
        for(lint i=0 ; i<netArr.size() ; ++i)
            if(netArr[i].distribute.first != 0 && netArr[i].distribute.second != 0)
                curCost++;
    }
}

void output()
{
    lint cost = 0;
    for(lint i=0 ; i<netArr.size() ; ++i)
        if(netArr[i].distribute.first != 0 && netArr[i].distribute.second != 0)
            cost++;
    fout << cost << '\n';
    fout << "2\n";
    for(lint i=0 ; i<cellArr.size() ; ++i)
        fout << cellArr[i].setID << '\n';
    fout.close();
}

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    fin.open(argv[1], ios::in);
    string outFileName(argv[1]);
    outFileName += ".out";
    fout.open(outFileName, ios::out);
    init();
    solve();
    output();
    return 0;
}
