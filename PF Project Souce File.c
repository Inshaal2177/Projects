#include <stdio.h>
#include <string.h>
int menufunction();
void votecast();
void  result(); 
void exitsystem(); 
char voter_cnic[20][20];
int vote_count[4]={0};
int voter_count=0;
int main()
{
	void (*menu[3])()={votecast,result,exitsystem};
	int choice=0;
	do
	{
		choice= menufunction();
	
		if(choice>=1&&choice<=3)
		{
			(*menu[choice-1])();	
		}else
		{
			printf("INVALID CHOICE!");
		}
	}
	while(choice!=3);
	return 0;
}
int menufunction()
{
	int choice=0;
	printf("\n-----------VOTING MANANGMENT SYSTEM-----------\n");
	printf("1. Cast vote\n2. Result\n3. Exit\n");
	printf("------------------------------------------------\n");
	printf("Enter Your Choice: \n");
	scanf("%d", &choice);
	return choice;
}
void votecast()
{
	int party, true = 1;
	char cnic[20];
	
	  printf("Enter Your CNIC:\n ");
	  scanf("%s", cnic);
	for(int i=0;i<20;i++)
	{
		while (true){
			if(strcmp(voter_cnic[i],cnic)==0)
       {
    	printf("CNIC ALREADY VOTED!\nDUPLICATE VOTES ARE NOT ALLOWED! \n");
    	printf("Enter CNIC Again! \n");
    	scanf("%s", cnic);
		}else{
			true =0;
		}
	}
}
	
    printf("Select Party Which You Want to vote:\n");
    printf("1.PTI\n");
    printf("2.PPP\n");
    printf("3.PMLN\n");
    printf("4.N-LEAGUE\n");
    scanf("%d", &party);

    if(party>=1&&party<=4)
    {
    	printf("\nVote Casted Sucessfully\n");
    	vote_count[party-1]++;
    	strcpy(voter_cnic[voter_count],cnic);
    	voter_count++;
    }else
	{
    	printf("\nInvalid number!\n");
	}
}
void result()
{
	int max=vote_count[0],winner;
	printf("\n=========VOTING RESULTS========\n");
	printf("Party PTI's votes:%d\n", vote_count[0]);
	printf("Party PPP's votes:%d\n", vote_count[1]);
	printf("Party PMLN's votes:%d\n", vote_count[2]);
	printf("Party NON-LEAGUE's votes:%d\n",vote_count[3]);
	printf("Total votes cast:%d\n",voter_count);
	printf("===================================\n");
	
    if(vote_count[0]== vote_count[1] && vote_count[1]== vote_count[2] && vote_count[2]== vote_count[3]){
    	
        printf("Its a TIE Between all Parties");
    }else if(vote_count[0]> vote_count[1] && vote_count[0] > vote_count[2] && vote_count[0]> vote_count[3]){
        printf("Winner is PTI with %d Votes", vote_count[0]);
    }else if(vote_count[1]> vote_count[0] && vote_count[1] > vote_count[2] && vote_count[1]> vote_count[3]){
        printf("Winner is PPP with %d Votes", vote_count[1]);
    }else if(vote_count[2]> vote_count[0] && vote_count[2] > vote_count[1] && vote_count[2]> vote_count[3]){
        printf("Winner is PMLN with %d Votes", vote_count[2]);
    }else if(vote_count[3]> vote_count[0] && vote_count[3] > vote_count[1] && vote_count[3]> vote_count[2]){
        printf("Winner is N-LEAGUE with %d Votes", vote_count[3]);
    }
    int tieCount = 0;
    for(int i = 0; i < 4; i++){
        if(vote_count[i] == max){
            tieCount++;
        }
    }
    switch(tieCount)
    {
        case 3:
            printf("It's a 3-Way TIE Between: \n");
            for(int i = 0; i < 4; i++){
                if(vote_count[i] == max)
                    printf("Party %d ", i+1);
            }
            printf("\n");
            break;
        case 2:
        	
            printf("It's a 2-Way TIE Between: \n");
            for(int i = 0; i < 4; i++){
                if(vote_count[i] == max)
                    printf("Party %d ", i+1);
            }
            printf("\n");
            break;
    }
}
void exitsystem()
{
	printf("\nExiting Voting System...\n");
	printf("\nThank you for Voting\n");
}

