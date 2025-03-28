# **📡 Client-Serveur TCP/UDP en C sous Unix**  

![Unix Network Programming](assets/unix-network.png)  

## **📌 Aperçu du Projet**  
Ce projet consiste en **deux applications client-serveur** développées en **C sous Unix/Linux**, utilisant les protocoles **TCP** (avec authentification) et **UDP** pour la communication réseau.  

### **🔹 Partie 1 : Communication UDP**  
- **Objectif** : Envoyer un entier `n` du client au serveur, qui renvoie un tableau de `n` nombres aléatoires triés.  
- **Protocole** : UDP (sans connexion, léger).  

### **🔹 Partie 2 : Communication TCP**  
- **Objectif** : Implémenter un serveur avec **authentification** et un menu interactif offrant des services comme :  
  - Obtenir la date/heure.  
  - Lister les fichiers d’un répertoire.  
  - Lire le contenu d’un fichier.  
  - Calculer le temps écoulé depuis la connexion.  
- **Protocole** : TCP (orienté connexion, fiable).  

---

## **⚙️ Fonctionnalités**  
### **Partie UDP**  
✅ Client génère un entier aléatoire `n` et l’envoie au serveur.  
✅ Serveur génère `n` nombres aléatoires, les trie, et les renvoie.  

### **Partie TCP**  
✅ **Authentification** (identifiants prédéfinis : `user` / `password`).  
✅ **Menu interactif** avec 4 services.  
✅ Gestion multi-clients via **processus fils**.  

---

## **🚀 Installation & Exécution**  
### **1. Compilation**  
```bash
# Compiler le client/serveur UDP
gcc udp_client.c -o udp_client
gcc udp_server.c -o udp_server

# Compiler le client/serveur TCP
gcc tcp_client.c -o tcp_client
gcc tcp_server.c -o tcp_server
```

### **2. Exécution**  
#### **UDP**  
```bash
# Lancer le serveur (écoute sur le port 8080)
./udp_server 8080

# Lancer le client (connecté à localhost:8080)
./udp_client 127.0.0.1 8080
```

#### **TCP**  
```bash
# Lancer le serveur (écoute sur le port 8080)
./tcp_server 8080

# Lancer le client (connecté à localhost:8080)
./tcp_client 127.0.0.1 8080
```

---

## **📂 Structure des Fichiers**  
```
unix-network-project/
├── udp_client.c       # Client UDP
├── udp_server.c       # Serveur UDP
├── tcp_client.c       # Client TCP (avec authentification)
├── tcp_server.c       # Serveur TCP (multi-clients)
├── README.md          # Ce fichier
└── assets/            # Captures d’écran/diagrammes
```

---

## **📊 Résultats Attendus**  
### **UDP**  
```
[Client] Envoi de n = 5
[Serveur] Reçu n = 5 → Génération de [3, 1, 4, 2, 5]
[Client] Reçu tableau trié : [1, 2, 3, 4, 5]
```

### **TCP**  
```
> Nom d’utilisateur : user
> Mot de passe : password
Connexion réussie !
1. Date/Heure
2. Liste fichiers
3. Lire fichier
4. Temps écoulé
Choix : 1
2024-04-01 14:30:00
```

---

## **🔍 Analyse Technique**  
| **Critère**       | **UDP**                          | **TCP**                          |  
|-------------------|----------------------------------|----------------------------------|  
| **Type**          | Sans connexion                   | Orienté connexion                |  
| **Fiabilité**     | ❌ Non garantie                  | ✅ Garantie (acquittements)      |  
| **Usage**         | Échanges ponctuels               | Sessions interactives            |  

---

## **📜 Licence**  
Ce projet est sous licence **MIT**. Voir le fichier [LICENSE](LICENSE).  

---

## **📧 Contact**  
- **Auteur** : [Votre Nom] – [votre@email.com]  
- **Superviseur** : [Nom du Superviseur]  

---

**🔗 Ressources Utiles**  
- [Documentation des sockets Unix](https://man7.org/linux/man-pages/man2/socket.2.html)  
- [Tutoriel TCP/UDP en C](https://beej.us/guide/bgnet/)  

--- 

**🚀 Prêt à tester ? Compilez et exécutez les clients/serveurs !**
