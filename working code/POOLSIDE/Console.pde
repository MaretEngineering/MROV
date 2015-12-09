class Console{
    //this class basically implements a very simple queue
    //next keeps track of the oldest string (next one to be replaced)
    
    String[] output;
    int next = 0;
    int lines;
    int x, y;
    int w, h;
    boolean wrap;

    Console(int lines, int x, int y){
        this.x = x; //x-cord
        this.y = y; //y-cord
        this.lines = lines;
        wrap = false; //non-wrapping constructor
        
        output = new String[lines];
        for(int i = 0; i < lines; i++){ //initialize all strings
            output[i] = "";
        }
    }

    Console(int lines, int x, int y, int w, int h){
        this.x = x; //x-cord
        this.y = y; //y-cord
        this.w = w; //width of wrap
        this.h = h; //height of wrap
        this.lines = lines;
        wrap = true; //wrapping constructor
        
        output = new String[lines];
        for(int i = 0; i < lines; i++){ //initialize all strings
            output[i] = "";
        }
    }
    
    public void addLine(String newLine){
        output[next] = newLine;
        next++;
        if(next >= lines){ //wrap around
            next = 0;
        }
    }

    public String getOutput(){
        String toReturn = "";
        for(int i = next; i < lines; i++){ //start at the real begining
            toReturn = toReturn + output[i] + "\n";
        }
        for(int i = 0; i < next; i++){ //do the rest
            toReturn = toReturn + output[i] + "\n";
        }
        
        return toReturn;
    }

    //x-cord, y-cord
    public void printOutput(){
        textAlign(LEFT, TOP);
        if(wrap)
            text(getOutput(), x, y, h, w);
        else
            text(getOutput(), x, y);
    }
    
    public void println(String str){
        addLine(str);
        printOutput();
    }
    
}
