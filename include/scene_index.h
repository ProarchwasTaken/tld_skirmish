// scene_index.h
#pragma once
#include <raylib.h>
#include <vector>
#include "base/generics.h"
#include "base/scene.h"
#include "data/data_index.h"
#include "hud_menu.h"
#include "game.h"

#define SKIR_ENTRY_PLR 0
#define SKIR_ENTRY_GOL 1
#define SKIR_ENTRY_WRH 2
#define SKIR_ENTRY_DAM 3


class IndexScene : public Scene {
public:
  IndexScene(Game &skirmish);
  ~IndexScene();

  void updateScene() override;
  void transitionLerp();
  void scrolling();

  void checkInput() override;
  void descInput(bool input_back, bool input_down, bool input_up,
                 bool gamepad_detected);

  void drawCursor(Vector2 position);
  void drawOptions();
  void drawEntryDetails();

  /* Due to how raylib works, I can't begin TextureMode for two render
   * textures at once. How I solved it is by momentarily ending 
   * TextureMode for the game canvas, and starting it back up when it's
   * time to draw the description.*/
  void drawEntryDescription(IndexData *entry, int font_size);
  void drawScene() override;
private:
  MenuHud menu_hud = MenuHud(false);
  RenderTexture desc_canvas;

  bool ready = false;
  bool exiting = false;

  float ready_time = 0.25;
  float ready_percentage = 0.0;

  float box_y = 69;
  Color box_tint = {255, 255, 255, 0};

  menu_options skir_options = {
    SKIR_ENTRY_PLR,
    SKIR_ENTRY_GOL,
    SKIR_ENTRY_WRH,
    SKIR_ENTRY_DAM
  };

  menu_options::iterator selected_option = skir_options.begin();

  bool desc_scrolling = false;
  bool scrolling_down = false;
  bool scrolling_up = false;

  float scroll_speed = 2.0;

  std::vector<IndexData> skir_entries = {
    {"SKIRMISHER", "HUMAN - MID TIER", "ALIVE - ACTIVE", 
      "LIFE: 30 - MORALE: 50\n"
      "---\n"
      "A human boy who seems to be in\n"
      "their mid-teens judging by his \n"
      "height, and build. I don't know his\n"
      "name, so I'll have to go with the\n"
      "placeholder name of 'Skirmisher'\n"
      "for now.\n\n"

      "He seems to be one of the few\n"
      "humans left who haven't succumbed\n"
      "to the terrible fate of being used\n"
      "as lifestock or losing their\n"
      "precious humanity. I assume he was\n"
      "lucky enough to remain hidden, and\n"
      "not be found over the course of\n"
      "these last two years. Albeit, the\n"
      "vampire's search for the remaining\n"
      "humans was extremely extensive, so\n"
      "it was inevitable that I would\n"
      "eventrully learn of his existence.\n\n"

      "I went ahead of the horde\n"
      "encroaching in on the human's\n"
      "current location to see him for\n"
      "myself. He was leaning against the\n"
      "wall... crying... My heart aches at the\n"
      "sight as I watched from the \n"
      "shadows.\n\n"

      "When the sounds of the incoming\n"
      "horde became audible, he wiped his\n"
      "tears away, and had this look in his\n"
      "eyes... A look I'm all too familiar\n"
      "with.\n\n"

      "I'm waiting for the perfect moment\n"
      "to reveal myself to him. If I did\n"
      "this wrong, he could try to resist\n"
      "like many others have futilely did\n"
      "before. I don't want to risk him\n"
      "hurting himself trying.\n"
      "---\n"
      "Experience: C\n"
      "Mental Fortitude: B\n"
      "Intelligence: C\n"
      "Cooperation: D\n"
      "Emotional Intelligence: C\n"
      "Planning: D\n"
      "Quick Thinking: A\n"
      "Versatility: B"
    },

    {"GHOUL", "EX-HUMAN", "IRRELEVANT", 
      "LIFE: 15 - MORALE: N/A\n"
      "---\n"
      "Turning a human into a vampire is\n"
      "actually is very delicate process.\n"
      "A Ghoul is what happens when that\n"
      "process is done hastily and\n"
      "recklessly.\n\n"

      "I have witnessed some humans try\n"
      "to turns themselves into vampires\n"
      "by injecting themselves with their\n"
      "blood. Only to turn into a mindless\n"
      "creature that carries zero \n"
      "resemblance to their former selves.\n"
      "I absolutely despise anyone who\n"
      "would throwaway their humanity like\n"
      "that.\n\n"

      "A Ghoul has way lower mental\n"
      "capacity than that of a human or\n"
      "any animal. This makes it very\n"
      "easy to control and use them as\n"
      "as minions. A single one is easy\n"
      "for a human to deal with on most\n"
      "occasions."
    },

    {"WRETCH", "EX-HUMAN", "IRRELEVANT", 
      "LIFE: 20 - MORALE: N/A\n"
      "---\n"
      "Soon enough, the vampires grown\n"
      "complacent and arrogant. When they\n"
      "realized they could have an army\n"
      "of mindless minions to do the work\n"
      "of hunting down the remaining free\n"
      "humans for them, they quickly got\n"
      "to work.\n\n"

      "The Wretch is the result of many\n"
      "experiments that were performed\n"
      "to make the most deadly minion.\n"
      "They have a high amount of\n"
      "strength and attack speed, and\n"
      "they also possess the ability to\n"
      "materialize any weapon they want.\n"
      "With a giant sword being their\n"
      "most preferred option to use\n"
      "against anyone whose unfortunate\n"
      "enough to encounter them."
    },
    {"DAMNED", "NON-HUMAN", "IRRELEVANT", 
      "LIFE: 20 - MORALE: N/A\n"
      "---\n"
      "Vampires now outnumber humans\n"
      "by such large margin that they're\n"
      "now considered an endangered\n"
      "species. Humans are starting to\n"
      "become few and far between. The\n"
      "vampires just can't help\n"
      "themselves from just endlessly\n"
      "gorging upon humans by the dozens\n"
      "with zero restraint. How pitiful.\n\n"

      "At one point they tried to make\n"
      "artifical humans using skeletons\n"
      "and blood-drained corpses as a\n"
      "base. The result was an utter\n"
      "mockery of what a human should\n"
      "be. I was so insulted, that I\n"
      "secretly disposed of some of them\n"
      "in disgust. How insolent of them to\n"
      "even attempt to recreate something\n"
      "as beautiful as humanity!\n\n"

      "They were completely useless in\n"
      "sustaining the vampires anyway! It\n"
      "had to be from the blood of a\n"
      "purely human being! They're nothing\n"
      "but parasites that feed off\n"
      "humanity! Why did they think this\n"
      "would work?\n\n"

      "Sorry... I made a promise to myself\n"
      "to at least act professional when\n"
      "writing these. So they decided to\n"
      "use the Damned as another minion\n"
      "for capturing humans. I can see\n"
      "the hatred in their eyes. They opt\n"
      "choke their victims to death rather\n"
      "than tear them apart with their\n"
      "claws."
    }
  };
};
