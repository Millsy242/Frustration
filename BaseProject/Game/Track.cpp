//
//  Track.cpp
//  BaseProject
//
//  Created by Daniel Harvey on 15/09/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//

#include "Track.hpp"
#include "Yaml.hpp"

void Track::Start()
{
    return;
}
void Track::LoadTexture(std::string filepath)
{
    EntityTexture.loadFromFile(filepath);
    EntitySprite.setTexture(EntityTexture);
    EntitySprite.setScale(scaleX,scaleY);
}
void Track::EntityUpdate()
{
    
    
    // Reset racing line
    for (int i = 0; i < RacingLine.points.size(); i++)
    {
        RacingLine.points[i] = CSpline.points[i];
        fDisplacement[i] = 0;
    }
    RacingLine.UpdateSplineProperties();

    for (int n = 0; n < iteration; n++)
    {
        for (int i = 0; i < RacingLine.points.size(); i++)
        {
            // Get locations of neighbour nodes
            sf::Vector2f pointRight = RacingLine.GetSplinePoint((i + 1) % RacingLine.points.size());
            sf::Vector2f pointLeft = RacingLine.GetSplinePoint((i + RacingLine.points.size() - 1) % RacingLine.points.size());
            sf::Vector2f pointMiddle = RacingLine.GetSplinePoint(i);

            // Create vectors to neighbours
            sf::Vector2f vectorLeft = { pointLeft.x - pointMiddle.x, pointLeft.y - pointMiddle.y };
            sf::Vector2f vectorRight = { pointRight.x - pointMiddle.x, pointRight.y - pointMiddle.y };

            // Normalise neighbours
            float lengthLeft = sqrtf(vectorLeft.x*vectorLeft.x + vectorLeft.y*vectorLeft.y);
            sf::Vector2f leftn = { vectorLeft.x / lengthLeft, vectorLeft.y / lengthLeft };
            float lengthRight = sqrtf(vectorRight.x*vectorRight.x + vectorRight.y*vectorRight.y);
            sf::Vector2f rightn = { vectorRight.x / lengthRight, vectorRight.y / lengthRight };

            // Add together to create bisector vector
            sf::Vector2f vectorSum = { rightn.x + leftn.x, rightn.y + leftn.y };
            float len = sqrtf(vectorSum.x*vectorSum.x + vectorSum.y*vectorSum.y);
            vectorSum.x /= len;
            vectorSum.y /= len;

            // Get point gradient and normalise
            sf::Vector2f g = CSpline.GetSplineGradient(i);
            float glen = sqrtf(g.x*g.x + g.y*g.y);
            g.x /= glen; g.y /= glen;

            // Project required correction onto point tangent to give displacment
            float dp = -g.y*vectorSum.x + g.x * vectorSum.y;

            // Shortest path
            if(distance)
            fDisplacement[i] += (dp * 0.3f);

            // Curvature
            if(curve)
            {
                fDisplacement[(i + 1) % RacingLine.points.size()] += dp * -0.2f;
                fDisplacement[(i - 1 + RacingLine.points.size()) % RacingLine.points.size()] += dp * -0.2f;
            }
        }

        // Clamp displaced points to track width
        for (int i = 0; i < RacingLine.points.size(); i++)
        {
            if (fDisplacement[i] >= TrackWidth) fDisplacement[i] = TrackWidth;
            if (fDisplacement[i] <= -TrackWidth) fDisplacement[i] = -TrackWidth;

            sf::Vector2f g = CSpline.GetSplineGradient(i);
            float glen = sqrtf(g.x*g.x + g.y*g.y);
            g.x /= glen; g.y /= glen;

            RacingLine.points[i].x = CSpline.points[i].x + -g.y * fDisplacement[i];
            RacingLine.points[i].y = CSpline.points[i].y + g.x * fDisplacement[i];
        }
    }

    RacingLine.UpdateSplineProperties();
    return;
}
void Track::Render(Window *window)
{
    window->draw(EntitySprite);
   // window->draw(temp);
    
    CSpline.DrawSelf(window,sf::Color::White,{2,2});
    RacingLine.DrawSelf(window,sf::Color::Magenta,{5,5});
}
sf::Vector2f Track::GetStart()
{
    return CSpline.GetSplinePoint(StartNode);
}
void Track::Input(sf::Event event)
{
    ImGui::Begin("track");
    ImGui::SliderInt("iterations", &iteration, 0, 100);
    ImGui::Checkbox("Curve", &curve);
    ImGui::Checkbox("Distance", &distance);
    ImGui::End();
    return;
}
void Track::Exit()
{
    return;
}
bool Track::LoadTrackFromFile(std::string filepath, sf::Vector2u ScreenSize)
{
    Yaml::Node root;
    try
    {
        Yaml::Parse(root, filepath.data());
    }
    catch(const Yaml::Exception e)
    {
        std::cout << "Exception " << e.Type() << ": " << e.what() << std::endl;
        return false; 
    }
    
    Width       = root["Width"].As<int>();
    Height      = root["Height"].As<int>();
    numnodes    = root["NumNodes"].As<int>();
    Sector1     = root["Sector1"].As<int>();
    Sector2     = root["Sector2"].As<int>();
    Sector3     = root["Sector3"].As<int>();
    StartNode   = root["StartLine"].As<int>();
    EndNode     = root["FinishLine"].As<int>();
    TrackWidth  = root["TrackWidth"].As<float>();
    
    scaleX = float(ScreenSize.x)/Width;
    scaleY = float(ScreenSize.y)/Height;
    
    float Tx,Ty;
    std::string name;
    for(int i{0};i<numnodes;i++)
    {
        name = "Node" + std::to_string(i);
        Tx = root[name + "_X"].As<int>() ;
        Ty = root[name + "_Y"].As<int>() ;
        CSpline.AddPoint(sf::Vector2f(Tx,Ty));
        LSpline.AddPoint(sf::Vector2f(Tx,Ty));
        RSpline.AddPoint(sf::Vector2f(Tx,Ty));
        RacingLine.AddPoint(sf::Vector2f(Tx,Ty));
    }
    CSpline.SetScale(scaleX, scaleY);
    RSpline.SetScale(scaleX, scaleY);
    LSpline.SetScale(scaleX, scaleY);
    RacingLine.SetScale(scaleX, scaleY);
    TrackWidth *= ((scaleX+scaleY)/2);
    for(int i{0}; i<numnodes;i++)
    {
        auto p1 = CSpline.GetSplinePoint(i);
        auto g1 = CSpline.GetSplineGradient(i);
        float glen = sqrt(g1.x*g1.x+g1.y*g1.y);
        
        LSpline.points[i].x = p1.x + TrackWidth * (-g1.y / glen);
        LSpline.points[i].y = p1.y + TrackWidth * ( g1.x / glen);
        RSpline.points[i].x = p1.x - TrackWidth * (-g1.y / glen);
        RSpline.points[i].y = p1.y - TrackWidth * (g1.x / glen);
        
        LSpline.UpdateSplineProperties();
        RSpline.UpdateSplineProperties();
        
       // float r = atan2(-g1.y, g1.x);
        //nodes[i].Line.setPoints({TrackWidth * sin(r) + p1.x, trackwidth * cos(r) + p1.y}, {-trackwidth * sin(r) + p1.x, -trackwidth * cos(r) + p1.y});
    }
    CSpline.UpdateSplineProperties();

    LoadTexture(root["ImagePath"].As<std::string>());
    
    CreateTrackOutline();
    
    return true;
}
sf::Color Track::getPixel(sf::Vector2f point)
{
    return sf::Color::Black;
}
void Track::CreateTrackOutline()
{
    sf::RenderTexture RT;
    RT.create(Width*scaleX, Height*scaleY);
    sf::Color mycolour;
   
    RT.clear(sf::Color::Transparent);
    RSpline.DrawSelf(&RT);
    LSpline.DrawSelf(&RT);
    
    
    for(float t{0}; t<(float)CSpline.points.size();t+=0.0005)
    {
        sw::Line myline(RSpline.GetSplinePoint(t),LSpline.GetSplinePoint(t));
        myline.setColor(sf::Color::White);
        RT.draw(myline);
    }
    RT.display();
    trackedge = RT.getTexture();
    
    Collision::CreateTextureAndBitmask(&trackedge); 
    
    temp.setTexture(trackedge);
}
