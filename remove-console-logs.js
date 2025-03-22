// Ce script peut être exécuté avec Node.js pour supprimer tous les console.log
// du projet.
const fs = require('fs');
const path = require('path');

function removeConsoleLogs(directoryPath) {
    // Lire tous les fichiers du répertoire
    const files = fs.readdirSync(directoryPath);
    
    files.forEach(file => {
        const filePath = path.join(directoryPath, file);
        const stats = fs.statSync(filePath);
        
        if (stats.isDirectory()) {
            // Si c'est un répertoire, appeler récursivement
            removeConsoleLogs(filePath);
        } else if (filePath.endsWith('.js')) {
            // Si c'est un fichier JavaScript
            let content = fs.readFileSync(filePath, 'utf8');
            
            // Supprimer tous les console.log
            const newContent = content.replace(/console\.log\([^)]*\);?/g, '');
            
            // Écrire le fichier mis à jour
            if (content !== newContent) {
                fs.writeFileSync(filePath, newContent);
                console.log(`Removed console.logs from: ${filePath}`);
            }
        }
    });
}

// Commencer par le répertoire docs
removeConsoleLogs('./docs'); 