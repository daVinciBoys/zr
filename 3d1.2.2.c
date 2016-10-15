float err[3],err1[3],gd,gp,item[3],item1[3],vel,dist,dist1,att[3],area[4],distmin,diff,SPS1[3],num,O[3];
int stato,i,c,t,n;
bool b,d,a;
ZRState mystate;




void init()
{
    stato=10;
    gp=0.35;
    t=0;
    b=0;
    SPS1[2]=0;
    a=1;
    for(i=0;i<3;i++) O[i]=0;
    n=2;
}






void loop()
{
    api.getMyZRState (mystate);
    game.getZone(area);
    if(t==0) d=bor();
    t++;
    if(b==1) { stato=2; game.dropSPS();}
    DEBUG(("Stato: %d",stato));
    
    
    switch(stato)
    {
        
        case 10:
           
            game.dropSPS(); 
            stato=20;
            
        break;
        
        
        case 20:
        
            gd=3;
            
            if(d==true)
            {
            SPS1[0]=-0.75;
            SPS1[1]=-0.1;
            SPS1[2]=0.45;
            }
            else
            {
            SPS1[0]=0.75;
            SPS1[1]=0.1;
            SPS1[2]=-0.45;
            }
            
            //api.setPositionTarget(SPS1);
            api.setPosGains(gp, 0 ,gd);
            api.setPositionTarget(SPS1);//pospid(SPS1);
            if(fabs(mystate[0])>0.57) 
            {game.dropSPS(); stato=1;}
        break;
        
        
        
        
        case 1:
                gd=2;
                distmin=100;
                c=10;
                
                for(i=0;i<n;i++)
                {
                    game.getItemLoc(item, i);
            
                    if(!game.itemInZone(i) && game.hasItem(i)==0)
                    {
                        mathVecSubtract (err, item, mystate, 3);
                        dist=mathVecMagnitude(err,3);
        
                        if(dist<distmin) 
                        {
                            distmin=dist;
                            c=i;
                        }
                    }
                }
            
                game.getItemLoc(item, c);
                vel=mathVecMagnitude(&mystate[3],3);
                
                mathVecSubtract (att, item, mystate, 3);
                mathVecNormalize (att, 3);
                api.setAttitudeTarget(att);
        
                mathVecSubtract (err, item, mystate, 3);
                dist=mathVecMagnitude(err,3);
                
                
        
                mathVecNormalize (err, 3);
                diff=mathVecInner(err, &mystate[6], 3);
                
                
                
            
                    if(c<2)        
                    {
                        num=0.155;
                        if(dist<0.173 && dist>0.155 && vel<0.01 && diff>0.97) b=game.dockItem();
                    }
                    if(c>1 && c<4) 
                    {
                        num=0.15;
                        if(dist<0.155 && dist>0.144 && vel<0.01 && diff>0.97) b=game.dockItem();
                    }
                    if(c>3)        
                    {
                        num=0.124;
                        if(dist<0.146 && dist>0.124 && vel<0.01 && diff>0.97) b=game.dockItem();
                    }
                    
                    dist1=dist-num;
                    if(dist1<0.15) if (vel>0.01) gd=4; else gd=2;
                    DEBUG(("diff:%f  dist:%f dist1:%f c:%d",diff,dist,dist1,c));
                    if(a==true) 
                    {
                        mathVecScale(err1, att, dist1);
                        mathVecAdd(item1,err1,mystate,3);
                        a=false;
                    }
                    
                    api.setPosGains(gp, 0 ,gd);
                    if(game.getFuelRemaining()<15 && dist>0.3) api.setPositionTarget(area);
                    else api.setPositionTarget(item1);//pospid(item1);
            
               
                
        break;
        
        case 2:
                mathVecSubtract (att, area, mystate, 3);
                mathVecNormalize (att, 3);
                api.setAttitudeTarget(att);
        
                mathVecSubtract (err, area, mystate, 3);
                dist=mathVecMagnitude(err,3);
        
                mathVecNormalize (err, 3);
                diff=mathVecInner(err, &mystate[6], 3);
                
                if(dist<0.2) gd=6;
                else gd=4;
                api.setPosGains(gp, 0 ,gd);
                
                if(dist<0.15)
                {
                    if(diff>0.98)
                    {
                        b=0;
                        game.dropItem();
                        stato=1;
                        a=true;
                        n=6;
                    }
                    api.setPositionTarget(O);
                    //pospid(O);
                }
                else  if(dist>0.15)  api.setPositionTarget(area);//pospid(area); //api.setPositionTarget(area);
                DEBUG(("dist:%f    diff:%f",dist,diff));
        
        break;
        
    }
    
    
    
}



bool bor()
{
    bool colore;
    if(mystate[1]>0) colore = true;
    else colore = false;
    
    return colore;
}

void mathVecScale(float res[3], float src[3], float mag)
{
    memcpy(res,src,sizeof(float)*3);
    res[0]*=mag;
    res[1]*=mag;
    res[2]*=mag;
}       
