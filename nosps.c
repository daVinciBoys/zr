float err[3],olderr[3],f[3],fp[3],fd[3],fi[3],gd,gp;
float item[3],vel,dist,mypos[3],att[3];
float area[4];
int upperlimit, lowerlimit,stato,i,t;
float oldstato;
float p1[3],p2[3],centro[3];

ZRState mystate;


void init()
{
    upperlimit=1;
    lowerlimit=-1;
    stato=0;
    gp=0.5;
    api.setPosGains( 0.5, 0 ,6);
    t=0;
    for(i=0;i<3;i++)
    {
        p1[i]=0;
        p2[i]=0;
    }
    
}

void loop()
{
    api.getMyZRState (mystate);
    game.getZone(area);
    t++;
    
    switch(stato)
    {
        
        case 0:    
                game.dropSPS();
                game.dropSPS();
                game.dropSPS();
                stato=1;
        break;
        
        case 1:
                    
                for(i=0;i<3;i++)
                {
                        if(area[i]>p1[i]) p1[i]=area[i]; 
                        else if(area[i]<p2[i]) p2[i]=area[i]; 
                        //DEBUG(("AREA%d:%f",i,area[i]));
                    item[i]=(p1[i]+p2[i])/2;
                }
                
                
                if(t>10) api.setPositionTarget(item);
                
        break;
        
    }
}
