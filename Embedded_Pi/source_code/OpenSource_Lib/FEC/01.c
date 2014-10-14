
#define 	Schar8  	signed  char       //windows环境下 跟 单片机有区别 
#define 	Uchar8  	unsigned char
#define 	Uint16 		unsigned short int
#define     Sint16      signed short int
#define 	Uint32 		unsigned int
#define 	Sint32 		signed int
#define     UINT16      unsigned short int
#define     UINT8       unsigned char
 
Uint16 Calc_CRCSUM(Uchar8 *FIFO_buff, Uchar8 n);
Uchar8 hanming(Uchar8 RX_sequence, Uchar8 likely);
void FEC_decoding(Uchar8 *RX_data, Uchar8 length);  

Uchar8  fecEncodeTable[] = 
{ 
     0, 3, 1, 2, 
     3, 0, 2, 1,
     3, 0, 2, 1,
     0, 3, 1, 2 
}; 

Uchar8 input[10]={3,1,2,3}; 
Uchar8 i;
Uchar8 j;
Uint16 val;
Uint16 fecReg;
Uint16 fecOutput; 
Uint32 intOutput; 
Uchar8 fec[20]; 
Uchar8 interleaved[20]; 
Uint16 inputNum = 0, fecNum;  // 16 bit 防止不够 
Uint16 checksum; 
Uchar8 length = 3; 


int main(int argc, char *argv[])
{
  
//------------------------------------------------------------------- 
    inputNum = length + 1;  
     
    printf("Input: [%5d bytes]\n", inputNum); 
    for (i = 0; i < inputNum; i++) 
    {
        printf("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " ");
    }     
    printf("\n\n");
     
    // Generate CRC 
    checksum = Calc_CRCSUM(input, 4);      
    input[inputNum++] = (checksum >> 8);   // CRC_H   原代码 少了 & 0x00FF   造成数据错误  input[4] = 0xDF130  原因是  windows环境下  unsigned int = 32bit  
    input[inputNum++] = checksum & 0x00FF; // CRC_L
    
    printf("Appended CRC: [%5d bytes]\n", inputNum); 
    for (i = 0; i < inputNum; i++) 
    {
        printf("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " "); 
    }        
    printf("\n\n"); 
    
    // Append Trellis Terminator 
    input[inputNum] = 0x0B; 
    input[inputNum + 1] = 0x0B; 
    fecNum = 2*((inputNum / 2) + 1); // inputNum为奇数  fecNum = inputNum + 1；偶数 + 2   不在循环不用优化 
    
    printf("Appended Trellis terminator: [%5d bytes]\n", fecNum); 
    for (i = 0; i < fecNum; i++) 
    {
        printf("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " ");  
    }        
    printf("\n\n"); 
  
    // FEC encode 
    fecReg = 0;  
    for (i = 0; i < fecNum; i++) 
    { 
         fecReg = (fecReg & 0x700) | (input[i] & 0xFF); // S2 S1 S0  data(8bit)   
         fecOutput = 0; 
         for (j = 0; j < 8; j++) 
         { 
             fecOutput = (fecOutput << 2) | fecEncodeTable[fecReg >> 7]; //查表状态机 
             fecReg = (fecReg << 1) & 0x7FF; 
         } 
         fec[i * 2] = fecOutput >> 8;  //fec_H
         fec[i * 2 + 1] = fecOutput & 0xFF; //fec_L 
    }
    //printf("%02X%s", fecReg & 0x700,"\n");
     
    printf("FEC encoder output: [%5d bytes]\n", fecNum * 2); 
    for (i = 0; i < fecNum * 2; i++) 
    { 
         printf("%02X%s", fec[i], (i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
    }      
    printf("\n\n"); 
    
    // TX Perform interleaving 
    for (i = 0; i < fecNum * 2; i += 4) 
    { 
        intOutput = 0; 
        for (j = 0; j < 4*4; j++)
        { 
           intOutput = (intOutput << 2) | ((fec[i +(~j & 0x03)] >> (2 *  ((j & 0x0C) >> 2) )) & 0x03);
           // (~j) & 0x03 == (~j) % 4;  ((j & 0x0C) >> 2) == j/4;   
           //printf("%0d%s", i +(~j & 0x03), (i % 16 == 15) ? "\n" : " "); //
           //printf("%02X%s",~j);  
        } 
        //printf("\n\n"); //
        
        interleaved[i] = (intOutput >> 24) & 0xFF; 
        interleaved[i + 1] = (intOutput >> 16) & 0xFF; 
        interleaved[i + 2] = (intOutput >> 8) & 0xFF; 
        interleaved[i + 3] = (intOutput >> 0) & 0xFF; 
    } 
    
    printf("TX Interleaver output: [%5d bytes]\n", fecNum * 2); 
    for (i = 0; i < fecNum * 2; i++) 
    { 
         printf("%02X%s", interleaved[i], (i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
    } 
    printf("\n\n"); 
    
    //printf("%0d%s",sizeof( unsigned short int));
    
//-----------------------------------------------------------------------------    
    //RX Perform interleaving 
    for (i = 0; i < fecNum * 2; i += 4) 
    { 
        intOutput = 0; 
        for (j = 0; j < 4*4; j++)
        { 
           intOutput = (intOutput << 2) | ((interleaved[i +(~j & 0x03)] >> (2 *  ((j & 0x0C) >> 2) )) & 0x03); //interleaved 
        } 
        //printf("\n\n"); //
        
        interleaved[i] = (intOutput >> 24) & 0xFF; 
        interleaved[i + 1] = (intOutput >> 16) & 0xFF; 
        interleaved[i + 2] = (intOutput >> 8) & 0xFF; 
        interleaved[i + 3] = (intOutput >> 0) & 0xFF; 
    } 
    
    printf("RX Interleaver output: [%5d bytes]\n", fecNum * 2); 
    for (i = 0; i < fecNum * 2; i++) 
    { 
         printf("%02X%s", interleaved[i], (i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
    } 
    printf("\n\n");  
//-----------------------------------------------------------------------------    
    
    interleaved[12] = 0x62; //原发送：0x00   先修改：0xYY  查看纠错能力  
                            //这步修改应该在接收反交织之前 才是真实情况 
    
    FEC_decoding(interleaved, 16);
 

    getchar();
     
  return 0; 
}

//Viterbi 解码  length < 255;  卷积译码后的length  发送时组针不能超过 127 byte 
void FEC_decoding(Uchar8 *RX_data, Uchar8 length) 
{
    Uint16 i;
	Uint16 j; 
	Uint16 error = 0;  //用得出的最优解的汉明距离value来代表错误的bit个数  
    Uchar8 *p;   //打印译码序列循环指针 
    Uchar8 temp1;
    Uchar8 temp2;
    Uchar8 temp3;
    Uchar8 temp; 
    //Uchar8 path[length];
    
    //当前S2,S1,S0状态下结算之前所走路径的汉明距离
    Uchar8 distance_000 = 0;   
	Uchar8 distance_001 = 0; 
	Uchar8 distance_010 = 0;
	Uchar8 distance_011 = 0; //
	Uchar8 distance_100 = 0;
	Uchar8 distance_101 = 0; 
	Uchar8 distance_110 = 0;
	Uchar8 distance_111 = 0; //
	
    //每一步计算比较后的汉明距离（临时） 
    Uchar8 d_000 = 0;   
	Uchar8 d_001 = 0; 
	Uchar8 d_010 = 0;
	Uchar8 d_011 = 0; //
	Uchar8 d_100 = 0;
	Uchar8 d_101 = 0; 
	Uchar8 d_110 = 0;
	Uchar8 d_111 = 0; // 
	 
    //以下时刻3结束时8条路径对应的译码值  时刻4时16条路径再做优化处理再往后重复 
    Uchar8 path_000[length];//={0}; //0 0 0  
    Uchar8 path_001[length];//={0x01}; //0 0 1
    Uchar8 path_010[length];//={0x02}; //0 1 0
    Uchar8 path_011[length];//={0x03}; //0 1 1    //每次 i 的值 
    Uchar8 path_100[length];//={0x04}; //1 0 0
    Uchar8 path_101[length];//={0x05}; //1 0 1
    Uchar8 path_110[length];//={0x06}; //1 1 0  
    Uchar8 path_111[length];//={0x07}; //1 1 1 
    
    path_000[0] = 0x00; //0 0 0  
    path_001[0] = 0x01; //0 0 1
    path_010[0] = 0x02; //0 1 0
    path_011[0] = 0x03; //0 1 1    //每次 i 的值 
    path_100[0] = 0x04; //1 0 0
    path_101[0] = 0x05; //1 0 1
    path_110[0] = 0x06; //1 1 0  
    path_111[0] = 0x07; //1 1 1  
    
    //每一步计算比较后的路径（临时） 
    Uchar8 p_000[length];
    Uchar8 p_001[length];
    Uchar8 p_010[length];
    Uchar8 p_011[length];
    Uchar8 p_100[length];
    Uchar8 p_101[length];
    Uchar8 p_110[length]; 
    Uchar8 p_111[length]; 
    
    //以下: 时刻3结束时8条路径对应的码距 
    #define HM(a,b)   ((( (a^b)+ 1 ) >> 1) & 0x03) 
    /*********************************************************************** 
      汉明距离：
	          a(RX_data), b(可能的路径) 
			  a ^ b :  00 01 10 11 (最低位 2bit)
                  x ：  0  1  2  3 (十进制表示 最低 2bit)                       
                  y :   0  1  1  2 (对应的汉明距离)
           x与y关系 ：  y=(x+1)/2  
                       
      注意：由于汉明距离调用的次数为 4 *length, 所以不用子函数调用，
	        节省进出子函数压栈保护和出栈的时间，改用宏定义。  					    
    ***********************************************************************/
    temp1 = ((*RX_data) & 0xC0) >>6;
    temp2 = (((*RX_data) << 2) & 0xC0) >>6;
    temp3 = (((*RX_data) << 4) & 0xC0) >>6;
    distance_000 = HM(temp1, 0x00) + HM(temp2, 0x00) + HM(temp3, 0x00); // 00-00-00  g1,g0 
    distance_001 = HM(temp1, 0x00) + HM(temp2, 0x00) + HM(temp3, 0x03); // 00-00-11 
    distance_010 = HM(temp1, 0x00) + HM(temp2, 0x03) + HM(temp3, 0x01); // 00-11-01 
    distance_011 = HM(temp1, 0x00) + HM(temp2, 0x03) + HM(temp3, 0x02); // 00-11-10  
    distance_100 = HM(temp1, 0x03) + HM(temp2, 0x01) + HM(temp3, 0x03); // 11-01-11 
    distance_101 = HM(temp1, 0x03) + HM(temp2, 0x01) + HM(temp3, 0x00); // 11-01-00 
    distance_110 = HM(temp1, 0x03) + HM(temp2, 0x02) + HM(temp3, 0x02); // 11-10-10 
    distance_111 = HM(temp1, 0x03) + HM(temp2, 0x02) + HM(temp3, 0x01); // 11-10-01
    
	//printf("%02X%s", distance_000,(i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
    //printf("%02X%s", distance_001,(i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
	//printf("%02X%s", distance_010,(i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
	//printf("%02X%s", distance_011,(i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
    //printf("%02X%s", distance_100,(i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
	//printf("%02X%s", distance_101,(i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " ");
	//printf("%02X%s", distance_110,(i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
    //printf("%02X%s", distance_111,(i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
      	
	#define FUN(dat,i) ((dat[i>>2] << ((i&0x03)<<1) & 0xc0)) >> 6 
	/************************************************************************ 
	  i >> 2 == i/4; (i&0x03)<<1 == (i%4) * 2   dat[x] = 0xC8 (11 00, 10 00) 
	  分别对 11 00 10 00 取出  丢给HM(a,b)计算汉明距离 
	*************************************************************************/
	 
	/*************************************************************************
	 幸存路径：先计算开始3步的汉明距离累积的8种情况，往后每走一步比较每个状态 
	           的两条可能路径，选择汉明距离小的路径作为幸存路径。 
			    
	           pathXXX代表结束状态 S2,S1,S0 == XXX, 所走过路径的译码值
	*************************************************************************/
    for (i=3; i < 4*16; i++)     
    {
        //000 状态选取幸存路径 
		temp = FUN(RX_data,i);
		temp1 = distance_000 + HM(temp,0x00); // 00  g1,g0
		temp2 = distance_100 + HM(temp,0x03); // 11  g1,g0 
		
	//	printf("%02X%s", temp, (i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
	//	printf("%02X%s", temp1,(i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
	//	printf("%02X%s", temp2,(i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
		 
        if ((temp1) < (temp2)) // 000 <-- 000  比较  000 <-- 100 (S2,S1,S0)   
        { 
            p_000[i>>3] = (path_000[i>>3] << 1) & 0xFE;  //赋0 
            //当前 S2,S1,S0(000)  path_XXX[x] = 0 (0xFE)   i/8 == i>>3  
            d_000 = temp1; // 跟新明汉距离，切记选择完了才更新 
        }
        else
        {         
            memcpy(p_000, path_100, i>>3); // cpopy之前走过的路径  
            p_000[i>>3] = (path_100[i>>3] << 1) & 0xFE; //跟新当前这一步走过的路径 
            d_000 = temp2; //
        }// end if  
 
        //001 状态选取幸存路径        
        temp = FUN(RX_data,i);
		temp1 = distance_000 + HM(temp,0x03); // 11  g1,g0
		temp2 = distance_100 + HM(temp,0x00); // 00  g1,g0
        if ((temp1) < (temp2)) // 001 <-- 000  比较  001 <-- 100 (S2,S1,S0)  
        {   			 	    
			memcpy(p_001, path_000, i>>3); // cpopy之前走过的路径
			         
            p_001[i>>3] = (path_000[i>>3] << 1) | 0x01; 
            d_001 = temp1;
        }
        else
        {			
			memcpy(p_001, path_100, i>>3); // copy之前走过的路径	    
            
            p_001[i>>3] = (path_100[i>>3] << 1) | 0x01; 
            d_001 = temp2; 
        }// end if

        //010 状态
        temp = FUN(RX_data,i);
		temp1 = distance_001 + HM(temp,0x01); // 01  g1,g0
		temp2 = distance_101 + HM(temp,0x02); // 10  g1,g0
        if ((temp1) < (temp2)) // 010 <-- 001  比较  010 <-- 101 (S2,S1,S0)   
        {			
			memcpy(p_010, path_001, i>>3); // cpopy之前走过的路径	    
				   
            p_010[i>>3] = (path_001[i>>3] << 1) & 0xFE; 
            d_010 = temp1; // 
        }
        else
        {    	 
			memcpy(p_010, path_101, i>>3); // cpopy之前走过的路径 	    
			         
            p_010[i>>3] = (path_101[i>>3] << 1) & 0xFE;
            d_010 = temp2; //
        }// end if 

        // 011状态路径
        temp = FUN(RX_data,i);
		temp1 = distance_001 + HM(temp,0x02); // 10  g1,g0
		temp2 = distance_101 + HM(temp,0x01); // 01  g1,g0
        if ((temp1) < (temp2)) // 011 <-- 001  比较  011 <-- 101 (S2,S1,S0)  
        {   
			memcpy(p_011, path_001, i>>3); // cpopy之前走过的路径	  	    
			         
            p_011[i>>3] = (path_001[i>>3] << 1) | 0x01; 
            d_011 = temp1;
        }
        else
        { 	 
			memcpy(p_011, path_101, i>>3); // cpopy之前走过的路径 	    
            
            p_011[i>>3] = (path_101[i>>3] << 1) | 0x01; 
            d_011 = temp2; 
        }// end if
        
        // 100 
        temp = FUN(RX_data,i);
		temp1 = distance_010 + HM(temp,0x03); // 11  g1,g0
		temp2 = distance_110 + HM(temp,0x00); // 00  g1,g0
        if ((temp1) < (temp2)) // 100 <-- 010  比较  100 <-- 110 (S2,S1,S0)   
        {  
			memcpy(p_100, path_010, i>>3); // cpopy之前走过的路径	    
				   
            p_100[i>>3] = (path_010[i>>3] << 1) & 0xFE; 
            d_100 = temp1; // 
        }
        else
        {   	
			memcpy(p_100, path_110, i>>3); // cpopy之前走过的路径  	    
			         
            p_100[i>>3] = (path_110[i>>3] << 1) & 0xFE;
            d_100 = temp2; //
        }// end if 
        
        // 101状态路径
        temp = FUN(RX_data,i);
		temp1 = distance_010 + HM(temp,0x00); // 00  g1,g0
		temp2 = distance_110 + HM(temp,0x03); // 11  g1,g0
        if ((temp1) < (temp2)) // 101 <-- 010  比较  101 <-- 110 (S2,S1,S0)  
        {   	  
			memcpy(p_101, path_010, i>>3); // cpopy之前走过的路径	    
			         
            p_101[i>>3] = (path_010[i>>3] << 1) | 0x01; 
            d_101 = temp1;
        }
        else
        {  
			memcpy(p_101, path_110, i>>3); // cpopy之前走过的路径	    
            
            p_101[i>>3] = (path_110[i>>3] << 1) | 0x01; 
            d_101 = temp2; 
        }// end if
        
        // 110 
        temp = FUN(RX_data,i);
		temp1 = distance_011 + HM(temp,0x02); // 10  g1,g0
		temp2 = distance_111 + HM(temp,0x01); // 01  g1,g0
        if ((temp1) < (temp2)) // 110 <-- 011  比较  100 <-- 111 (S2,S1,S0)   
        {  	
			memcpy(p_110, path_011, i>>3); // cpopy之前走过的路径  	    
				   
            p_110[i>>3] = (path_011[i>>3] << 1) & 0xFE; 
            d_110 = temp1; // 
        }
        else
        {    
			memcpy(p_110, path_111, i>>3); // cpopy之前走过的路径	    
			         
            p_110[i>>3] = (path_111[i>>3] << 1) & 0xFE;
            d_110 = temp2; //
        }// end if 
        
        // 111状态路径
        temp = FUN(RX_data,i);
		temp1 = distance_011 + HM(temp,0x01); // 01  g1,g0
		temp2 = distance_111 + HM(temp,0x02); // 10  g1,g0
        if ((temp1) < (temp2)) // 111 <-- 011  比较  111 <-- 111 (S2,S1,S0)  
        {   
			memcpy(p_111, path_011, i>>3); // cpopy之前走过的路径  	    
			         
            p_111[i>>3] = (path_011[i>>3] << 1) | 0x01; 
            d_111 = temp1;
        }
        else
        {
		 /*	for (j=0; j<(i>>3); j++)
            {
                path_111[j]=path_111[j];
            }  */	  	    
             
            p_111[i>>3] = (path_111[i>>3] << 1) | 0x01; 
            d_111 = temp2; 
        }// end if
        
        //更新路径、汉明距离
        distance_000 = d_000;   
	    distance_001 = d_001; 
	    distance_010 = d_010;
        distance_011 = d_011; //
	    distance_100 = d_100;
	    distance_101 = d_101; 
	    distance_110 = d_110;
        distance_111 = d_111; //  
		 
        memcpy(path_000, p_000, 16); // cpopy之前走过的路径
        memcpy(path_001, p_001, 16); // cpopy之前走过的路径
        memcpy(path_010, p_010, 16); // cpopy之前走过的路径
        memcpy(path_011, p_011, 16); // cpopy之前走过的路径
        memcpy(path_100, p_100, 16); // cpopy之前走过的路径
        memcpy(path_101, p_101, 16); // cpopy之前走过的路径
        memcpy(path_110, p_110, 16); // cpopy之前走过的路径
        memcpy(path_111, p_111, 16); // cpopy之前走过的路径
        
    }//end for  16条路径 cutdown 8条路径
    
    
	
    // 最后一步，在四条路径中选择汉明距离最小的一条
    error = distance_000;
    p = path_000;  //取地址 
	    
    if (distance_001 < error)
    {
        error = distance_001;
        p = path_001;
    }
    if (distance_010 < error)       //luXX 存储路径 
    {
        error = distance_010;
        p = path_010;
    }
    if (distance_011 < error)
    {
        error = distance_011;
        p = path_011;
    }
    if (distance_100 < error)
    {
        error = distance_100;
        p = path_100;
    }
    if (distance_101 < error)       //luXX 存储路径 
    {
        error = distance_101;
        p = path_101;
    }
    if (distance_110 < error)
    {
        error = distance_110;
        p = path_110;
    }
    if (distance_110 < error)
    {
        error = distance_110;
        p = path_110;
    }
    if (distance_111 < error)       //luXX 存储路径 
    {
        error = distance_111;
        p = path_111;  
    }
    
     
    printf("共有%d位错误，译码如下：\n",error);
    for (i=0; i<8; i++)
    {
        printf("%02X%s", *p, (i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
        p++;
    }    
    		  
}

//计算xy两个2bit数的汉明距离    汉明子函数调用次数过于频繁  后期进一步优化 
Uchar8 hanming(Uchar8 RX_sequence, Uchar8 likely) //左接收data 右需要比较的每一步可能值 
{   
    //改写成宏定义 #define HM(a,b)   ((Uchar8)((( (a^b) +1) & 0xC0) >> 1) & 0xC0))
    Uchar8 sum = 0;
    Uchar8 temp = 0;
    
    temp = RX_sequence ^ likely;
    temp = temp & 0x03;   // 00XX  
	 
    while (temp) 
    { 
        sum = sum + (temp & 0x01); 
        temp = temp >> 1; 
    }//end while  (sum = 汉明距离) 
    
    return sum; 
} //子函数改成宏定义，减少子函数压栈保护盒出栈的时间 !!!

// CRC 软件解码  输入CRC_reg = CRC种子  输出 CRC_reg = CRC 校验和 
Uint16 Calc_CRCSUM(Uchar8 *FIFO_buff, Uchar8 n)
{
     Uchar8 i;
     Uchar8 x;
     Uint16 CRC_SUM;
     Uchar8 dat;
     
     CRC_SUM = 0xFFFF; //CRC_SUM的初始值 = 0xFFFF，相关内容查看DN502 
     
     for (x=0; x<n; x++)
     {   
         dat = *(FIFO_buff + x);  //导入FIFO_buff 数组数据
         
         for (i=0; i<8; i++)
         {
             if (((CRC_SUM & 0x8000) >> 8) ^ (dat & 0x80))
             {
                CRC_SUM = (CRC_SUM << 1) ^ 0x8005;  //CRC_16算法   0x8005 查看DN095
             }
             else
             {
                CRC_SUM = (CRC_SUM << 1);
             }//end if    
             dat = dat << 1;
         }// 1 byte Clac 
     }//end for  CRC_SUM 
     
     return CRC_SUM;
}
