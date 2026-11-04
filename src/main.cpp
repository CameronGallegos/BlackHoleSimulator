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
std::vector<sf::CircleShape> applyGravLensing(std::vector<sf::CircleShape>, double, double);


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

        //need this for lensing
        sf::Vector2f blackholePosition = star.getPosition();
        std::vector<sf::CircleShape> lensedStars = applyGravLensing(backgroundStars, blackholePosition.x, blackholePosition.y);



        //this will be at the bottom b/c we want to do all the math and fancy stuff and then draw what we calculated
        //clear window and fill with black...b/c its in space 
        window.clear(sf::Color::Black);
        //draw background first because main star can draw over it
        for(int i = 0; i < lensedStars.size(); i++){
            window.draw(lensedStars[i]);
        }
        window.draw(star);        
        window.display(); //this will display waht we drew before


    }



    return 0;
}


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

//
std::vector<sf::CircleShape> applyGravLensing(
    std::vector<sf::CircleShape> backgroundStars,
    double blackHoleX,
    double blackHoleY){
    
    //just simplify this instead of the full calcllation
    double lensingStrength = 1000.0;

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

        // cant divide by 0
        if (distance < 1.0) {
            continue;
        }

        //how much the star should shift
        double shift = lensingStrength / distance;

        // Find the direction away from the black hole
        double directionX = (starPosition.x - blackHoleX) / distance;
        double directionY = (starPosition.y - blackHoleY) / distance;

        // Calculate the new position
        float newX = static_cast<float>(starPosition.x + directionX * shift);
        float newY = static_cast<float>(starPosition.y + directionY * shift);

        // Move the background star
        backgroundStars[i].setPosition({newX, newY});
    }

    return backgroundStars;
}