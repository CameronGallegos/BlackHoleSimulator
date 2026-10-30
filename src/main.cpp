#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <random>

int main(){
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;
    const float centerX = windowWidth / 2.0;
    const float centerY = windowHeight / 2.0;
    const float starRadOG = 30.0;
    //below will be our math constants
    // constexpr is like a super const, cannot change whatsoever or it is an error
    constexpr double G = 6.67430e-11;
    constexpr double c = 299792458.0;
    constexpr double metersPerPixel = 500.0; //star really small, lets try 500
    double starMass = 1.989e30; //mass of the sun but will change during real sims
    double schwarzschildRadius = (2*G*starMass) / (c*c);
    double schwarzschildRadiusPixels = schwarzschildRadius / metersPerPixel;

    sf::RenderWindow window; //so this is from the sfml. it is like an int or string but will open a window for us to mess wit
    //we could also have done below above but w/o using the .create because it would have used the constructor
    //for the 800x600. this is best for little projects, 1024x768 is a larger window, 1280x720 is for games, 1920x1080 is for full hd
    window.create(sf::VideoMode({windowWidth, windowHeight}), "Black Hole Simulator");

    //window open now make a star
    sf::CircleShape star(starRadOG); //circle is already apart of sfml, 30 is radius, f is for a float
    //now that we have variables for changeablility we need to change our logic
    star.setPosition({centerX - star.getRadius(), centerY - star.getRadius()});
    sf::Clock clock; //built in object from sfml, will help with the star disappearing almost instantly

    //all the background stars
    std::vector<sf::CircleShape> backgroundStars;
    //okay now this part is not gonna look pretty... the rando stuff
    std::mt19937 randomNumG(std::random_device{}()); //so mt generates random numbers, random dev gives a diff seed each time we run it
    std::uniform_real_distribution<float> randomX(0.0f, static_cast<float>(windowWidth));
    std::uniform_real_distribution<float> randomY(0.0f, static_cast<float>(windowHeight));
    for(int i = 0; i <200; i++){
        sf::CircleShape littleStar(1.0f);
        littleStar.setPosition({randomX(randomNumG), randomY(randomNumG)});
        backgroundStars.push_back(littleStar);
    }

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

        //so here we want the star to actually collapse, so we shrink until we get to our blackHole radius oooouuuu
        if(star.getRadius() > schwarzschildRadiusPixels){
            //star shrinks 5 pixels per second now
            //our max function prevents it from being a negative value
            //this got a bit messy but i think all can understand it
            star.setRadius(std::max(
                static_cast<float>(schwarzschildRadiusPixels), 
                star.getRadius() - 5.0f * timeChange
            ));
        }


        //you would think == would be preferred here but some simulations did not work...something to do with precision i expect
        if(star.getRadius() <= static_cast<float>(schwarzschildRadiusPixels)){
            star.setFillColor(sf::Color::Black); //this line is quite obvious
            star.setOutlineColor(sf::Color::White); 
            star.setOutlineThickness(2.0f);
        }






        //this will be at the bottom b/c we want to do all the math and fancy stuff and then draw what we calculated
        //clear window and fill with black...b/c its in space 
        window.clear(sf::Color::Black);
        //draw background first because main star can draw over it
        for(int i = 0; i < backgroundStars.size(); i++){
            window.draw(backgroundStars[i]);
        }
        window.draw(star);        
        window.display(); //this will display waht we draw before


    }



    return 0;
}