class Solution {
public:
    vector<string> fullJustify(vector<string>& words, int maxi) {
      int max_size=0;
      int word_size=0;
      vector<string>temp;
      int start=0;
      int count=-1;
        for(int i=0;i<words.size();i++){
          string s;
          word_size+=words[i].size();
          max_size+=words[i].size();
          if(i==words.size()-1){
              s=words[start];
             int spaces=maxi-max_size;
             for(int j=start+1;j<=i;j++){
                s+=' ';
                s+=words[j];
             }
             for(int j=1;j<=spaces;j++){
               s+=' ';
             }
          }
          if(max_size>maxi){
            int size=word_size-words[i].size();
            i--;
            s=words[start];
            if(count!=0){
            int rem=(maxi-size) % (count);
            for(int j=start+1;j<=start+count;j++){
               if(rem==0){
                 for(int k=1;k<=(maxi-size)/count;k++){
                     s+=' ';
                 }
                 s+=words[j];
                 
               }
               else{
                   for(int k=1;k<=((maxi-size)/count)+1;k++){
                     s+=' ';   
                 }
                 s+=words[j];
                 rem--;
               }
               
           }
           }
           else{
             for(int j=1;j<=(maxi-words[start].size());j++){
              s+=' ';
             }
           }
           word_size=0;
           max_size=0;
           count=-1;
           start=i+1;
          }
          else if(max_size==maxi){
            s=words[start];
             for(int j=start+1;j<=i;j++){
                s+=' ';
                s+=words[j];
             }
             word_size=0;
             max_size=0;
             count=-1;
             start=i+1;
          }
          else{
            max_size++;
            count++;
          }
         if(s.size())
         temp.push_back(s);
        
        }
        return temp;
    }
};