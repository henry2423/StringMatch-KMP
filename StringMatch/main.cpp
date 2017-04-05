//
//  main.cpp
//  StringMatch
//
//  Created by Henry on 14/11/2016.
//  Copyright © 2016 henry. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sys/time.h>

// 程式架構 -> 先把一行的pattern讀入 然後再跟所有的text進行比對 接著再控制text的指標回到開頭 然後接著讀入下一行pattern 再跟所有text進行比對

using namespace std;

int main(int argc, const char * argv[]) {
    
    struct timeval t_val;
    gettimeofday(&t_val, NULL);
    /*---要計算的程式效率區域---*/
    
    
    char text[2000] = {0};    //text = search words
    char pat[2000] = {0};    //pat = keywords
    int line = 1;   //第幾行
    int sizeof_pat = 0;     //運算patter的長度
    int sizeof_text = 0;    //運算text的長度
    bool has_result = 0;    //紀錄是否有匹配成功
    int failure[2000] = {0};      //failure function
    
    
    if(argc<=2)     //確定會參數設定正確
    {
        cout<<"File Setting is not complete."<<endl;
        return 1;
    }
    
    fstream pat_file;      //fstream 讀入pattern file
    fstream text_file;     //fstream 讀入text file
    pat_file.open(argv[1],ios::in);
    text_file.open(argv[2],ios::in);
    
    //確定有讀到檔案 沒有的話就跳出程式
    if(!pat_file)
    {
        cout<<"Can't open the file"<<endl;
        return 1;
    }
    if(!text_file)
    {
        cout<<"Can't open the file"<<endl;
        return 1;
    }
    
    
    //先從第一個pat開始search 把所有組text搜完再換下一組pat
    while(!pat_file.eof())
    {
        pat_file.getline(pat, sizeof(pat));    //取得一行的pat
        for(int i=0; pat[i]!='\0'; i++) //計算pat的長度
        {
            sizeof_pat = i+1;
        }
        
        //實做failure Function
        int j=0; //neddle to make search
        for(int i=1; i<sizeof_pat; i++)
        {
            //Fail
            while( pat[i]!=pat[j] )
            {
                if(j!=0)    //退找下一個Failure
                    j = failure[j-1];
                else        //當Failure退無可退的時候就是0 跳出while
                {
                    failure[j] = 0;
                    break;
                }
            }
            
            //Success
            if( pat[i]==pat[j] )
            {
                failure[i] = j+1;   //設定failure j+1
                j = failure[i];     //然後設定j準備看下一個pat的字母是否能比對成功
            }
            
            
        }
        
        /*  看 Failure function 的數字
         for(int i=0;i<sizeof_pat;i++)
         {
         cout << failure[i] << " ";
         }
         cout<<endl;
         */
        
        //開始讀text的文字
        while(!text_file.eof())
        {
            text_file.getline(text, sizeof(text)); //取得一行的text
            for(int i=0; text[i]!='\0'; i++) //計算text的長度
            {
                sizeof_text = i+1;
            }
            
            
            //實作KMP演算法
            int index=0; //text index
            int neddle=0; //neddle to make search
            while( index < sizeof_text )
            {
                
                //Fail
                if( tolower(text[index]) != tolower(pat[neddle]) )
                {
                    if(neddle!=0)    //尋找比對失敗的上一個字元的Failure function
                        neddle = failure[neddle-1];
                    else            //直接跳下一個字母比對
                    {
                        index++;
                    }
                    continue;
                }
                
                //Success (Full examine complete)
                if( neddle == sizeof_pat-1)
                {
                    has_result = 1;     //一個flag代表有結果了
                    
                    //輸出結果
                    printf("%d %d \"",line,index-neddle+1);
                    for(int i=0; i<sizeof_pat; i++)
                    {
                        printf("%c",pat[i]);
                    }
                    printf("\" \"");
                    
                    for(int i=0; i<sizeof_text; i++)
                    {
                        printf("%c",text[i]);
                    }
                    printf("\"\n");
                    
                    
                    //Success之後不能直接全部跳鍋 說不定小距離移動之後還是有比對成功的可能 所以要再看failure function
                    if(neddle == 0)   //如果字串移動的距離=0 就直接強制移動
                        index++;
                    else
                        neddle = failure[neddle-1];
                    continue;
                }
                
                //一個字母比對成功 繼續檢查下一個字元是否相同
                index++;
                neddle++;
            }
            
            
            line++;     //換text的下一行跟此pat做比對
        }
        
        
        text_file.clear();  // 重置 eof
        text_file.seekg(0);  // get 指標移至檔案首
        line = 1;      //因為換新的pat 需要重新讀text檔所以把line再設定成1
    }
    
    //關閉檔案
    pat_file.close();
    text_file.close();
    
    //如果都比對不到的話就輸出這行
    if(!has_result)
        printf("No match string in this test. \n");
    
    //時間計算
    struct timeval t_val_end;
    gettimeofday(&t_val_end, NULL);
    struct timeval t_result;
    timersub(&t_val_end, &t_val, &t_result);    //開始跟結束時間相減
    double consume = t_result.tv_sec*1000 + (1.0 * t_result.tv_usec)/1000;  //換算成毫秒
    printf("%f ms \n", consume);
    
    
    
    
    return 0;
}
