

day : date '\n' day_body
    ;

day_body : day_body_element day_body
         |
         ;

day_body_element : ref 
                 | task
                 ;

ref : refid reftitle refdesc emptyline
    ;

refdesc
