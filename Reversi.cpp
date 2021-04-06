#include <bits/stdc++.h>
using namespace std;

#define INFINITY 999999
#define DEPTH 5

int row,column;

int weight1[8][8]={ {4,-3,2,2,2,2,-3,4},
                    {-3,-4,-1,-1,-1,-1,-4,-3},
                    {2,-1,1,0,0,1,-1,2},
                    {2,-1,0,1,1,0,-1,2},
                    {2,-1,0,1,1,0,-1,2},
                    {2,-1,1,0,0,1,-1,2},
                    {-3,-4,-1,-1,-1,-1,-4,-3},
                    {4,-3,2,2,2,2,-3,4} };

int weight2[8][8]={ {100,-20,10,5,5,10,-20,100},
                    {-20,-50,-2,-2,-2,-2,-50,-20},
                    {10,-2,-1,-1,-1,-1,-2,10},
                    {5,-2,-1,-1,-1,-1,-2,5},
                    {5,-2,-1,-1,-1,-1,-2,5},
                    {10,-2,-1,-1,-1,-1,-2,10},
                    {-20,-50,-2,-2,-2,-2,-50,-20},
                    {100,-20,10,5,5,10,-20,100} };

class Othello
{
public:
    int board[8][8];
    int depth;
    int player;
    int count[8][8];

    Othello(int pl,int de)
    {
        player=pl;
        depth=de;
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
            {
                count[i][j]=0;
            }
        }
    }
    void validMove();
    void performMove(int pos1,int pos2);
    int validMoveCount();
};

int heuristic1(Othello &node)
{
    int sum=0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            sum=sum+(node.board[i][j]*weight2[i][j]);
        }
    }
    return sum;
}

int heuristic4(Othello &node)
{
    int sum=0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            sum=sum+(node.board[i][j]*weight1[i][j]);
        }
    }
    return sum;
}

int heuristic2(Othello &node)
{
    int heu=0,cPlayer=0,cOpponent=0,mPlayer=0,mOpponent=0;

    node.validMove();
    mPlayer=node.validMoveCount();

    node.player=-node.player;
    node.validMove();
    mOpponent=node.validMoveCount();
    node.player=-node.player;
    node.validMove();

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(i==0 || j==0 || i==7 || j==7)
            {
                if(node.board[i][j]==node.player)
                {
                    mPlayer++;
                }
                else if(node.board[i][j]==-node.player)
                {
                    mOpponent++;
                }
            }
        }
    }

    heu = 10*(cPlayer-cOpponent) + 1*(mPlayer-mOpponent)/(mPlayer+mOpponent);

    return heu;
}

int heuristic3(Othello &node)
{
    int pl1=0,pl2=0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(node.board[i][j]==node.player)
            {
                pl1++;
            }
            else if(node.board[i][j]==-node.player)
            {
                pl2++;
            }
        }
    }
    return pl1-pl2;
}

int Othello::validMoveCount()
{
    int c=0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(count[i][j]!=0)
            {
                c++;
            }
        }
    }
    return c;
}

void makeCountZero(Othello &node)
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            node.count[i][j]=0;
        }
    }
}

void Othello::validMove()
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            count[i][j]=0;
        }
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j]==player)
            {
                //search up
                if(board[i-1][j]==-player && i>0)
                {
                    for(int x=i-2;x>=0;x--)
                    {
                        if(board[x][j]==-player)
                        {
                            continue;
                        }
                        else if(board[x][j]==player)
                        {
                            break;
                        }
                        else if(board[x][j]==0)
                        {
                            count[x][j]++;
                            break;
                        }
                    }
                }

                //search down
                if(board[i+1][j]==-player && i<7)
                {
                    for(int x=i+2;x<8;x++)
                    {
                        if(board[x][j]==-player)
                        {
                            continue;
                        }
                        else if(board[x][j]==player)
                        {
                            break;
                        }
                        else if(board[x][j]==0)
                        {
                            count[x][j]++;
                            break;
                        }
                    }
                }

                //search right
                if(board[i][j+1]==-player && j<7)
                {
                    for(int x=j+2;x<8;x++)
                    {
                        if(board[i][x]==-player)
                        {
                            continue;
                        }
                        else if(board[i][x]==player)
                        {
                            break;
                        }
                        else if(board[i][x]==0)
                        {
                            count[i][x]++;
                            break;
                        }
                    }
                }

                //search left
                if(board[i][j-1]==-player && j>0)
                {
                    for(int x=j-2;x>=0;x--)
                    {
                        if(board[i][x]==-player)
                        {
                            continue;
                        }
                        else if(board[i][x]==player)
                        {
                            break;
                        }
                        else if(board[i][x]==0)
                        {
                            count[i][x]++;
                            break;
                        }
                    }
                }

                //search diagonal up-right
                if(board[i-1][j+1]==-player && j<7 && i>0)
                {
                    int x=i-2,y=j+2;
                    for(;x>=0 && y<8 ;x--,y++)
                    {
                        if(board[x][y]==-player)
                        {
                            continue;
                        }
                        else if(board[x][y]==player)
                        {
                            break;
                        }
                        else if(board[x][y]==0)
                        {
                            count[x][y]++;
                            break;
                        }
                    }
                }

                //search diagonal up-left
                if(board[i-1][j-1]==-player && j>0 && i>0)
                {
                    int x=i-2,y=j-2;
                    for(;x>=0 && y>=0 ;x--,y--)
                    {
                        if(board[x][y]==-player)
                        {
                            continue;
                        }
                        else if(board[x][y]==player)
                        {
                            break;
                        }
                        else if(board[x][y]==0)
                        {
                            count[x][y]++;
                            break;
                        }
                    }
                }

                //search diagonal down-left
                if(board[i+1][j-1]==-player && j>0 && i<7)
                {
                    int x=i+2,y=j-2;
                    for(;x<8 && y>=0 ;x++,y--)
                    {
                        if(board[x][y]==-player)
                        {
                            continue;
                        }
                        else if(board[x][y]==player)
                        {
                            break;
                        }
                        else if(board[x][y]==0)
                        {
                            count[x][y]++;
                            break;
                        }
                    }
                }

                //search diagonal down-right
                if(board[i+1][j+1]==-player && j<7 && i<7)
                {
                    int x=i+2,y=j+2;
                    for(;x<8 && y<8 ;x++,y++)
                    {
                        if(board[x][y]==-player)
                        {
                            continue;
                        }
                        else if(board[x][y]==player)
                        {
                            break;
                        }
                        else if(board[x][y]==0)
                        {
                            count[x][y]++;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void Othello::performMove(int pos1,int pos2)
{
    board[pos1][pos2]=player;


    //search up
    if(board[pos1-1][pos2]==-player && pos1>0)
    {
        for(int x=pos1-2;x>=0;x--)
        {
            if(board[x][pos2]==-player)
            {
                continue;
            }
            else if(board[x][pos2]==player)
            {
                for(;x<pos1;x++)
                {
                    board[x][pos2]=player;
                }
                break;
            }
            else if(board[x][pos2]==0)
            {
                break;
            }
        }
    }

    //search down
    if(board[pos1+1][pos2]==-player && pos1<7)
    {
        for(int x=pos1+2;x<8;x++)
        {
            if(board[x][pos2]==-player)
            {
                continue;
            }
            else if(board[x][pos2]==player)
            {
                for(;x>pos1;x--)
                {
                    board[x][pos2]=player;
                }
                break;
            }
            else if(board[x][pos2]==0)
            {
                break;
            }
        }
    }

    //search right
    if(board[pos1][pos2+1]==-player && pos2<7)
    {
        for(int x=pos2+2;x<8;x++)
        {
            if(board[pos1][x]==-player)
            {
                continue;
            }
            else if(board[pos1][x]==player)
            {
                for(;x>pos2;x--)
                {
                    board[pos1][x]=player;
                }
                break;
            }
            else if(board[pos1][x]==0)
            {
                break;
            }
        }
    }

    //search left
    if(board[pos1][pos2-1]==-player && pos2>0)
    {
        for(int x=pos2-2;x>=0;x--)
        {
            if(board[pos1][x]==-player)
            {
                continue;
            }
            else if(board[pos1][x]==player)
            {
                for(;x<pos2;x++)
                {
                    board[pos1][x]=player;
                }
                break;
            }
            else if(board[pos1][x]==0)
            {
                break;
            }
        }
    }

    //search diagonal up-right
    if(board[pos1-1][pos2+1]==-player && pos2<7 && pos1>0)
    {
        int x=pos1-2,y=pos2+2;
        for(;x>=0 && y<8 ;x--,y++)
        {
            if(board[x][y]==-player)
            {
                continue;
            }
            else if(board[x][y]==player)
            {
                for(;x<pos1 && y>pos2;x++,y--)
                {
                    board[x][y]=player;
                }
                break;
            }
            else if(board[x][y]==0)
            {
                break;
            }
        }
    }

    //search diagonal up-left
    if(board[pos1-1][pos2-1]==-player && pos2>0 && pos1>0)
    {
        int x=pos1-2,y=pos2-2;
        for(;x>=0 && y>=0 ;x--,y--)
        {
            if(board[x][y]==-player)
            {
                continue;
            }
            else if(board[x][y]==player)
            {
                for(;x<pos1 && y<pos2;x++,y++)
                {
                    board[x][y]=player;
                }
                break;
            }
            else if(board[x][y]==0)
            {
                break;
            }
        }
    }

    //search diagonal down-left
    if(board[pos1+1][pos2-1]==-player && pos2>0 && pos1<7)
    {
        int x=pos1+2,y=pos2-2;
        for(;x<8 && y>=0 ;x++,y--)
        {
            if(board[x][y]==-player)
            {
                continue;
            }
            else if(board[x][y]==player)
            {
                for(;x>pos1 && y<pos2;x--,y++)
                {
                    board[x][y]=player;
                }
                break;
            }
            else if(board[x][y]==0)
            {
                break;
            }
        }
    }

    //search diagonal down-right
    if(board[pos1+1][pos2+1]==-player && pos2<7 && pos1<7)
    {
        int x=pos1+2,y=pos2+2;
        for(;x<8 && y<8 ;x++,y++)
        {
            if(board[x][y]==-player)
            {
                continue;
            }
            else if(board[x][y]==player)
            {
                for(;x>pos1 && y>pos2;x--,y--)
                {
                    board[x][y]=player;
                }
                break;
            }
            else if(board[x][y]==0)
            {
                break;
            }
        }
    }
}

void copyBoard(Othello &parent,Othello &child)
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            child.board[i][j]=parent.board[i][j];
        }
    }
}



int minimax(Othello &node, bool willMinimize, int alpha, int beta)
{
    srand(time(NULL));
    makeCountZero(node);
    node.validMove();
    int validMoves=node.validMoveCount(),heuristic;

    if(node.depth==DEPTH || validMoves==0)
    {
        if(node.player==1)
            heuristic=heuristic4(node);
        else if(node.player==-1)
            heuristic=heuristic3(node);
        return heuristic;
    }

    if(willMinimize)
    {
        int bestval=-INFINITY;

        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
            {
                if(node.count[i][j]!=0)
                {
                    Othello childNode(node.player, node.depth+1);
                    copyBoard(node,childNode);
                    childNode.performMove(i,j);
                    childNode.player=-childNode.player;

                    heuristic = minimax(childNode,false,alpha,beta);
                    if(bestval<heuristic && childNode.depth==1)
                    {
                        row=i;
                        column=j;
                    }
                    bestval = max(bestval,heuristic);
                    alpha = max(alpha,bestval);

                    if(beta<=alpha)
                    {
                        break;
                    }
                }
            }
        }
        return bestval;
    }
    else
    {
        int bestval=INFINITY;

        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
            {
                if(node.count[i][j]!=0)
                {
                    Othello childNode(node.player, node.depth+1);
                    copyBoard(node,childNode);
                    childNode.performMove(i,j);
                    childNode.player=-childNode.player;

                    heuristic = minimax(childNode,true,alpha,beta);
                    bestval = min(bestval,heuristic);
                    beta = min(beta,bestval);

                    if(beta<=alpha)
                    {
                        break;
                    }
                }
            }
        }
        return bestval;
    }
}

void printBoard(Othello &node)
{
    cout<<"    0   1   2   3   4   5   6   7"<<endl;
    for(int i=0;i<8;i++)
    {
        cout<<"  ---------------------------------"<<endl;
        cout<<i<<" ";
        for(int j=0;j<8;j++)
        {
            if(node.count[i][j]!=0)
            {
                cout<<"| ? ";
            }
            else if(node.board[i][j]==0)
            {
                cout<<"|   ";
            }
            else if(node.board[i][j]==1)
            {
                cout<<"| * ";
            }
            else if(node.board[i][j]==-1)
            {
                cout<<"| + ";
            }
        }
        cout<<"|"<<endl;
    }
    cout<<"  ---------------------------------"<<endl;
}

void printSuggestion(Othello &node)
{
    cout<<"    0   1   2   3   4   5   6   7"<<endl;
    for(int i=0;i<8;i++)
    {
        cout<<"  ---------------------------------"<<endl;
        cout<<i<<" ";
        for(int j=0;j<8;j++)
        {
            if(node.count[i][j]==0)
                cout<<"|   ";
            else
                cout<<"| ? ";
        }
        cout<<"|"<<endl;
    }
    cout<<"  ---------------------------------"<<endl;
}

bool terminate(Othello &node)
{
    int c=0,pl1=0,pl2=0,move1,move2;

    node.validMove();
    move1=node.validMoveCount();
    node.player=-node.player;
    node.validMove();
    move2=node.validMoveCount();
    node.player=-node.player;

    if(move1==0 && move2==0)
    {
        //cout<<"ekhane duitai 0 so break"<<endl<<endl;
        return true;
    }

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(node.board[i][j] == 1 )
            {
                pl1++;
            }
            else if(node.board[i][j]==-1)
            {
                pl2++;
            }
            else
            {
                c++;
            }
        }
    }
    if(pl1==0 || pl2==0)
    {
        return true;
    }
    if(c==0)
    {
        return true;
    }
    return false;
}

int main()
{
    Othello root(1,0);

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            root.board[i][j]=0;
        }
    }
    root.board[3][3]=1;
    root.board[4][4]=1;
    root.board[3][4]=-1;
    root.board[4][3]=-1;

    root.validMove();
    printBoard(root);


    /*//extra
    root.board[3][3]=1;
    root.board[3][4]=1;
    root.board[2][4]=1;
    root.board[4][4]=1;
    root.board[4][3]=-1;
    printBoard(root);
    root.player=-root.player;
    root.validMove();

    printSuggestion(root);

    minimax(root,true,-INFINITY,INFINITY);

    cout<<row<<" "<<column<<endl;*/

    while(!terminate(root))
    {
        /*//player 1
        int r,c;
        cout<<"Enter your move position : ";
        cin>>r>>c;
        if(root.count[r][c]==0)
        {
            cout<<"Wrong move."<<endl;
            continue;
        }
        root.performMove(r,c);
        makeCountZero(root);
        root.player=-root.player;
        root.validMove();

        printBoard(root);

        int pl1=0,pl2=0;
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
            {
                if(root.board[i][j]==1)
                {
                    pl1++;
                }
                else if(root.board[i][j]==-1)
                {
                    pl2++;
                }
            }
        }
        cout<<"Player star: "<<pl1<<endl<<"Player plus: "<<pl2<<endl;

        if(terminate(root))
        {
            break;
        }*/

        //cpu1

        minimax(root,true,-INFINITY,INFINITY);

        root.performMove(row,column);
        makeCountZero(root);
        root.player=-root.player;
        root.validMove();

        cout<<"CPU1 gives move ( "<<row<<", "<<column<<" )"<<endl;
        int pl1=0,pl2=0;
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
            {
                if(root.board[i][j]==1)
                {
                    pl1++;
                }
                else if(root.board[i][j]==-1)
                {
                    pl2++;
                }
            }
        }
        cout<<"Player star: "<<pl1<<endl<<"Player plus: "<<pl2<<endl;

        printBoard(root);

        if(terminate(root))
        {
            break;
        }

        //cpu2

        minimax(root,true,-INFINITY,INFINITY);

        root.performMove(row,column);
        makeCountZero(root);
        root.player=-root.player;
        root.validMove();

        cout<<"CPU2 gives move ( "<<row<<", "<<column<<" )"<<endl;
        pl1=0,pl2=0;
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
            {
                if(root.board[i][j]==1)
                {
                    pl1++;
                }
                else if(root.board[i][j]==-1)
                {
                    pl2++;
                }
            }
        }
        cout<<"Player star: "<<pl1<<endl<<"Player plus: "<<pl2<<endl;

        printBoard(root);

    }
    int pl1=0,pl2=0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(root.board[i][j]==1)
            {
                pl1++;
            }
            else if(root.board[i][j]==-1)
            {
                pl2++;
            }
        }
    }
    cout<<"Player star: "<<pl1<<endl<<"Player plus: "<<pl2<<endl;
    if(pl1>pl2)cout<<"**Player star Wins**"<<endl;
    else if(pl1<pl2)cout<<"**Player plus Wins**"<<endl;
    else cout<<"**DRAW**"<<endl;

    return 0;
}
