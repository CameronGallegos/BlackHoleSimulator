#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <random>
#include <cmath>

//funct headers will go here
double calculateSchwarzschildRadius(double);
std::vector<sf::CircleShape> randomBackgroundStars(unsigned int, unsigned int);
float shrinkStar(float, float, float, float);
double calculateDistance(double, double, double, double);
std::vector<sf::CircleShape> applyGravLensing(std::vector<sf::CircleShape>, double, double, 
                                                double, std::vector<sf::VertexArray>&);


int main(){

    //some work needs to be done here to clean this up
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;
    const float centerX = windowWidth / 2.0;
    const float centerY = windowHeight / 2.0;
    const float starRadOG = 30.0;
    constexpr double metersPerPixel = 200.0; //star really small, lets try 500
    float shrinkSpeed = 5.0f; //might tweak so created a new variable
    double starMass = 1.989e30; //mass of the sun but will change during real sims
    //could create new vari but well see
    double schwarzschildRadiusPixels = calculateSchwarzschildRadius(starMass) / metersPerPixel;

    sf::RenderWindow window; //so this is from the sfml. it is like an int or string but will open a window for us to mess wit
    //we could also have done below above but w/o using the .create because it would have used the constructor
    //for the 800x600. this is best for little projects, 1024x768 is a larger window, 1280x720 is for games, 1920x1080 is for full hd
    window.create(sf::VideoMode({windowWidth, windowHeight}), "Black Hole Simulator");

    //window open now make a star
    sf::CircleShape star(starRadOG); //circle is already apart of sfml, 30 is radius, f is for a float
    star.setOrigin({star.getRadius(), star.getRadius()});
    star.setPosition({centerX, centerY});

    sf::Clock clock; //built in object from sfml, will help with the star disappearing almost instantly

    //will call in draw section 
    std::vector<sf::CircleShape> backgroundStars = randomBackgroundStars(windowWidth, windowHeight);

    while(window.isOpen()){

        //we want to measure time so we can make it change more to how we want.
        //so we reset it and turn it into seconds so we can manipulate it
        //so line below is incorrect, reset jsut resets clock to 0
        //float timeChange = clock.reset().asSeconds();
        float timeChange = clock.restart().asSeconds();//this one is correct


        //so optional either points to the event happening or nothing(no event going on)
        //must use while b/c multiple events can be happening, stored as FIFO in optional 
        while (const std::optional<sf::Event> event = window.pollEvent()){
            //so this is like "hey is the event the user closing the window? yes? okay close it"
            if (event->is<sf::Event::Closed>()) { //event handling
                window.close();
            }
        }

        //so here we will be "setting" it...shrinking it 
        star.setRadius(shrinkStar(star.getRadius(), 
                        static_cast<float>(schwarzschildRadiusPixels),
                        timeChange, shrinkSpeed));

        star.setOrigin({star.getRadius(), star.getRadius()});

        //you would think == would be preferred here but some simulations did not work...something to do with precision i expect
        if(star.getRadius() <= static_cast<float>(schwarzschildRadiusPixels)){
            star.setFillColor(sf::Color::Black); //this line is quite obvious
            star.setOutlineColor(sf::Color::White); 
            star.setOutlineThickness(2.0f);
        }

        //V2f is just a vecotr taht is very convenient for holding x and y
        sf::Vector2f blackholePosition = star.getPosition();
        double lensingStrength = 100000.0 * (1.0 - (star.getRadius() / starRadOG));
        std::vector<sf::VertexArray> arcs;
        std::vector<sf::CircleShape> lensedStars = applyGravLensing(backgroundStars, blackholePosition.x, blackholePosition.y, 
                                                        lensingStrength, arcs);



        //this will be at the bottom b/c we want to do all the math and fancy stuff and then draw what we calculated
        //clear window and fill with black...b/c its in space 
        window.clear(sf::Color::Black);
        //draw background first because main star can draw over it
        for(int i = 0; i < lensedStars.size(); i++){
            window.draw(lensedStars[i]);
        }
        //now lets draw the arcs
        for(int i = 0; i < arcs.size(); i++){
            window.draw(arcs[i]);
        }
        window.draw(star);        
        window.display(); //this will display waht we drew before


    }
    return 0;
}

//self explanatory, just needed to clean up main
double calculateSchwarzschildRadius(double starMass){
    //below will be our math constants
    // constexpr is like a super const, cannot change whatsoever or it is an error
    constexpr double G = 6.67430e-11;
    constexpr double c = 299792458.0;
    double schwarzschildRadius = (2*G*starMass) / (c*c);
    return schwarzschildRadius;
}


//ugly funct to create 200 randomly placed background stars
std::vector<sf::CircleShape> randomBackgroundStars(unsigned int windowWidth, unsigned int windowHeight){
    //all the background stars
    std::vector<sf::CircleShape> stars;
    //okay now this part is not gonna look pretty... the rando stuff
    std::mt19937 randomNumG(std::random_device{}()); //so mt generates random numbers, random dev gives a diff seed each time we run it
    std::uniform_real_distribution<float> randomX(0.0f, static_cast<float>(windowWidth));
    std::uniform_real_distribution<float> randomY(0.0f, static_cast<float>(windowHeight));
    for(int i = 0; i <200; i++){
        sf::CircleShape littleStar(1.0f);
        littleStar.setPosition({randomX(randomNumG), randomY(randomNumG)});
        stars.push_back(littleStar);
    }
    return stars;
}


//
float shrinkStar(float currentRad, float schwarzschildRadiusPixels, float timeChange, float shrinkSpeed){

    //so here we want the star to actually collapse, so we shrink until we get to our blackHole radius oooouuuu
    if(currentRad > schwarzschildRadiusPixels){
        //star shrinks based on whatever we decide shrinkSpeed is (tweakable)
        //our max function prevents it from being a negative value
        //this got a bit messy but i think all can understand it
        return std::max(schwarzschildRadiusPixels,
            currentRad - (shrinkSpeed * timeChange)
        );
    }

    //if we are smaller or equal to the radius, dont keep going
    else
        return currentRad;

}
 
//distance stars are from blackhole to determine gravitational lensing
double calculateDistance(double starX, double starY, double blackHx, double blackHy){
    //just the euclidean distance formula :)
    return std::sqrt(std::pow((starX - blackHx), 2) + std::pow((starY - blackHy), 2));
}

//this one is ugly and long and i hope it is the hardest part of the project
std::vector<sf::CircleShape> applyGravLensing(
    std::vector<sf::CircleShape> backgroundStars,
    double blackHoleX,
    double blackHoleY,
    double lensStrength,
    std::vector<sf::VertexArray>& arcs){
    

    for (size_t i = 0; i < backgroundStars.size(); i++) {

        // Get the original position of the star
        sf::Vector2f starPosition = backgroundStars[i].getPosition();

        // Calculate distance from the black hole
        double distance = calculateDistance(
            starPosition.x,
            starPosition.y,
            blackHoleX,
            blackHoleY
        );

        //cant divide by 0
        if (distance < 1.0) {
            continue;
        }

        //if the star is farther away, it should shift less than one much closer
        double shift = lensStrength / pow(distance, 2); //we want further away ones to not move
        //this should stop us from launching stars into other galxies, whcih we are doing rn
        shift = std::min(shift, 30.0); //so the max travel distance is 30 pixels now

        //this tells us exact pos away from the blackhole
        double directionX = (starPosition.x - blackHoleX) / distance;
        double directionY = (starPosition.y - blackHoleY) / distance;

        //so we want to curve the star around the black hole so we need angles
        double angle = std::atan2(directionY, directionX);
        double arcWidth = 0.15; //about 8 degrees
        double arcBeginning = angle - (arcWidth / 2.0);
        double arcEnd = angle + (arcWidth / 2.0);

        //below will be the beginning and end points of the arcs of the stars
        //formula is  x = xcenter + r*cos(theta) or sin for y
        double arcBeginningX = blackHoleX + (distance * std::cos(arcBeginning));
        double arcBeginningY = blackHoleY + (distance * std::sin(arcBeginning));
        double arcEndX = blackHoleX + (distance * std::cos(arcEnd));
        double arcEndY = blackHoleY + (distance * std::sin(arcEnd));

        //okay so now we have to connect our arc points 
        sf::VertexArray arc(sf::PrimitiveType::LineStrip, 2 );
        //so we are setting pos to beginning and end
        arc[0].position = {static_cast<float> (arcBeginningX),
                            static_cast<float> (arcBeginningY)};

        arc[1].position = {static_cast<float> (arcEndX),
                            static_cast<float> (arcEndY)};

        //we want to store these arcs in our vector
        // we were drawing lines all over the place and it looked horrible, we only want close ones
        if(distance < 100.0){
            arcs.push_back(arc);
        }
        // Calculate the new position
        float newX = static_cast<float>(starPosition.x + directionX * shift);
        float newY = static_cast<float>(starPosition.y + directionY * shift);

        // Move the background star
        backgroundStars[i].setPosition({newX, newY});
    }

    return backgroundStars;
}