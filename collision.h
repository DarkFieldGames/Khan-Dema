// Collision detection.

bool collidingBoxes (core::vector3df a, core::vector3df b, float n, float h, core::vector3df A, core::vector3df B, float N, float H ) {
  
/* Interpolating collision checking. Looks at the starting point and ending point for both objects, and checks the space in between.
   Some concessions have been made for efficiency. But these'll only be noticed for oddly diagonal collisions, and we shouldn't have too many of those.

Each collision object has a starting position (ax,ay), an ending position (bx,by), and a size (w,h).

       y2
  ------------
  |        bb|
  |        bb|
  |        bb|
x1|          |x2
  |aa        |
  |aa        |
  |aa        |
  ------------
      y1

Kludgy bugfix: sometimes rocks are created at (0,0,0) before moving to their proper position.
*/
if (a == core::vector3df(0,0,0) 
 or b == core::vector3df(0,0,0)
 or A == core::vector3df(0,0,0)
 or B == core::vector3df(0,0,0)
) {return false;}

/*
First, we define one of these rectangles for each object.
*/
    float w [2] = {0.5*n,0.5*h};
    float W [2] = {0.5*N,0.5*H};

    float F[4]; // first object.  x1,x2,y1,y2
    float S[4]; // second object. X1,X2,Y1,Y2
  
    /* Slightly more optimised way of saying:
    X1 = min(Ax-W, Bx-W);
    X2 = max(Ax+W, Bx+W);
    for each X and Y. 
    At least, I think it's more optimised. Might be worth checking. */
    if (a.X<b.X) { F[0]=a.X-w[0]; F[1]=b.X+w[0]; } 
    else         { F[0]=b.X-w[0]; F[1]=a.X+w[0]; };
    if (a.Y<b.Y) { F[2]=a.Y-w[1]; F[3]=b.Y+w[1]; } 
    else         { F[2]=b.Y-w[1]; F[3]=a.Y+w[1]; };
    if (A.X<B.X) { S[0]=A.X-W[0]; S[1]=B.X+W[0]; } 
    else         { S[0]=B.X-W[0]; S[1]=A.X+W[0]; };
    if (A.Y<B.Y) { S[2]=A.Y-W[1]; S[3]=B.Y+W[1]; } 
    else         { S[2]=B.Y-W[1]; S[3]=A.Y+W[1]; };
  
    // Debug print-statements.
    /*
    cout << F[0] << "\t ";
    cout << F[1] << "\t ";
    cout << F[2] << "\t ";
    cout << F[3] << "\t ";
    cout << endl;
    cout << S[0] << "\t ";
    cout << S[1] << "\t ";
    cout << S[2] << "\t ";
    cout << S[3] << "\t ";
    cout << endl;
    
    cout << (S[1]<F[0]) << "\t";
    cout << (F[1]<S[0]) << "\t";
    cout << (S[3]<F[2]) << "\t";
    cout << (F[3]<S[2]) << endl; 
    if (not ( 
//  If one box is completely to one side of the other, then no collision.
        (S[1]<F[0] or F[1]<S[0]) // check X values. 
      or (S[3]<F[2] or F[3]<S[2]) // check Y values.
    )) {
      cout << "TRUE" << endl;
    } else {
      cout << "FALSE" << endl;
    };
  */  

//  a and b should all be the coordinates of the rectangles (x1,x2,y1,y2).
//  Now we check if they overlap.
    return not ( 
//  If one box is completely to one side of the other, then no collision.
//      !(
            (S[1]<F[0] or F[1]<S[0]) // check X values. 
         or (S[3]<F[2] or F[3]<S[2]) // check Y values.
//      )
    );
}
