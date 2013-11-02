


// Le système de fichier MyFileSystem a un superblock de 1024 octets au départ du disque logique
// Il peut se trouver éventuellement à un certain offset du début.
// Ce superblock détaille notamment le nombre de block total du disque et d'autres informations comme la version du
// Système de fichier.

// Chaque fichier sur le disque est représenté par un identifiant ID. Le fichier n'est pas
// continu dans la mémoire de stockage de masse. On fragmente le fichier en block de 1024 octets dans lesquels on retrouve
// L'ID du fichier et ses données utiles. Dans le premier block du fichier, on trouve sa taille utile ainsi que la taille
// du nom du fichier en octets (codé utf8, le max est 255).

// On retrouve également des blocks qui serviront à faire le lien entre l'ID du fichier et l'@ physique (en taille
// de SIZE_BLOCK octets) où se trouve
// le dit fichier. Ces blocks de 1024 octets sont reconnu grâce à leur desc_type à 1, ils sont chainés au bloc suivant
// Pour pouvoir étendre le nombre d'entrées dans la table



#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H


class FileSystem
{
public:
	//FileSystem();

	bool isGood() { return true; }
	// Renvoie l'identifiant
	//static int createFile(const char *name);
	//static void removeFile(int id) = 0;

	void openFile(int id);
	void closeFile(int id);
private:
};

#endif

