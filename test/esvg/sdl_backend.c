	SDL_Init(SDL_INIT_VIDEO);

	if (!(surface = SDL_SetVideoMode(w, h, 32, SDL_RESIZABLE | SDL_SRCALPHA | SDL_DOUBLEBUF)))
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		SDL_Quit();
		return NULL;
	}
	
