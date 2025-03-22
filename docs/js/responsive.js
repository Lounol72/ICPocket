// Assurer une meilleure structure pour le menu mobile
document.addEventListener('DOMContentLoaded', function() {
  const mobileMenuToggle = document.getElementById('mobile-menu-toggle');
  const nav = document.querySelector('nav');
  
  if (mobileMenuToggle && nav) {
    // Créer l'icône pour le toggle
    const toggleIcon = document.createElement('i');
    toggleIcon.className = 'fas fa-bars';
    mobileMenuToggle.innerHTML = '';
    mobileMenuToggle.appendChild(toggleIcon);
    
    // Déplacer le bouton toggle à l'intérieur de la nav pour l'effet Dynamic Island
    if (!nav.contains(mobileMenuToggle)) {
      const originalToggle = mobileMenuToggle.cloneNode(true);
      mobileMenuToggle.parentNode.removeChild(mobileMenuToggle);
      nav.prepend(originalToggle);
      
      // Mettre à jour la référence
      const newToggle = nav.querySelector('#mobile-menu-toggle');
      
      // Gérer le clic sur le toggle
      newToggle.addEventListener('click', function() {
        if (nav.classList.contains('mobile-active')) {
          // Fermer la Dynamic Island
          nav.style.animation = 'contractIsland 0.35s cubic-bezier(0.42, 0, 0.58, 1) forwards';
          
          // Changer l'icône
          const icon = this.querySelector('i');
          if (icon) icon.className = 'fas fa-bars';
          
          // Masquer tous les éléments sauf le toggle
          Array.from(nav.children).forEach(child => {
            if (child.id !== 'mobile-menu-toggle') {
              child.style.display = 'none';
            }
          });
          
          // Attendre la fin de l'animation avant de masquer
          setTimeout(() => {
            nav.classList.remove('mobile-active');
            nav.style.animation = '';
          }, 350);
        } else {
          // Ouvrir la Dynamic Island
          nav.classList.add('mobile-active');
          
          // Changer l'icône
          const icon = this.querySelector('i');
          if (icon) icon.className = 'fas fa-times';
          
          // Afficher tous les boutons de navigation
          Array.from(nav.children).forEach(child => {
            if (child.id !== 'mobile-menu-toggle') {
              // Délai pour que les boutons apparaissent après l'animation
              setTimeout(() => {
                child.style.display = 'flex';
              }, 150);
            }
          });
        }
      });
    }
    
    // Style initial pour la nav (semblable à la Dynamic Island fermée)
    if (window.innerWidth < 768) {
      // Afficher la navbar fermée sans contenu
      nav.style.display = 'flex';
      nav.style.height = 'fit-content';
      nav.style.minHeight = '40px';
      nav.style.justifyContent = 'center';
      
      // Masquer tous les éléments sauf le toggle
      Array.from(nav.children).forEach(child => {
        if (child.id !== 'mobile-menu-toggle') {
          child.style.display = 'none';
          child.style.justifyContent = 'center';
        }
      });
      
      // Activer le toggle
      const toggle = nav.querySelector('#mobile-menu-toggle');
      if (toggle) {
        toggle.style.display = 'block';
        toggle.style.height = 'fit-content';
      }
    }
  }
  
  // Gérer le redimensionnement
  window.addEventListener('resize', function() {
    if (window.innerWidth >= 768) {
      // Réinitialiser les styles en mode desktop
      if (nav) {
        nav.style.animation = '';
        nav.style.display = '';
        nav.style.height = '';
        
        Array.from(nav.children).forEach(child => {
          child.style.display = '';
        });
      }
    } else if (nav && !nav.classList.contains('mobile-active')) {
      // Rétablir l'apparence Dynamic Island en mode mobile
      nav.style.display = 'flex';
      nav.style.height = 'fit-content';
      
      Array.from(nav.children).forEach(child => {
        if (child.id !== 'mobile-menu-toggle') {
          child.style.display = 'none';
        }
      });
    }
  });
}); 