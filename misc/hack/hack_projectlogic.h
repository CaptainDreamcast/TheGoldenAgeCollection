#pragma once

void resetHackProject();
int getHackCodeProjectValue();
int getHackDesignProjectValue();
int getHackMarketingProjectValue();
int getHackTime();

void changeHackProjectCode(int tDelta);
void changeHackProjectDesign(int tDelta);
void changeHackProjectMarketing(int tDelta);
void changeHackProjectTime(int tDelta);
int isHackTimeOver();