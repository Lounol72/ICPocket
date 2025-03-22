/**
 * Ajouts professionnels pour ICPocket
 * Animation, interactions et fonctionnalités avancées
 */

document.addEventListener('DOMContentLoaded', function() {
    // 1. Effet de défilement sur la navigation
    window.addEventListener('scroll', function() {
        const nav = document.querySelector('nav');
        if (window.scrollY > 50) {
            nav.classList.add('scrolled');
        } else {
            nav.classList.remove('scrolled');
        }
    });
    
    // 2. Système de notification
    window.showNotification = function(message, type = 'info', duration = 5000) {
        // Créer la notification
        const notification = document.createElement('div');
        notification.className = `notification ${type}`;
        notification.innerHTML = `
            <button class="close-btn"><i class="fas fa-times"></i></button>
            <div class="notification-content">${message}</div>
        `;
        
        // Ajouter au DOM
        document.body.appendChild(notification);
        
        // Afficher avec animation
        setTimeout(() => notification.classList.add('show'), 10);
        
        // Configurer le bouton de fermeture
        const closeBtn = notification.querySelector('.close-btn');
        closeBtn.addEventListener('click', () => {
            notification.classList.remove('show');
            setTimeout(() => notification.remove(), 500);
        });
        
        // Auto-fermeture après durée spécifiée
        if (duration) {
            setTimeout(() => {
                notification.classList.remove('show');
                setTimeout(() => notification.remove(), 500);
            }, duration);
        }
        
        return notification;
    };
    
    // 3. Remplacer les animations de chargement par des skeletons quand c'est pertinent
    function replaceLoadingAnimations() {
        const loadingElements = document.querySelectorAll('.loading-animation');
        
        loadingElements.forEach(element => {
            // Vérifier si on est dans un conteneur de releases ou commits
            const isReleaseContainer = element.closest('#releases-container');
            const isCommitContainer = element.closest('#commits-container');
            
            if (isReleaseContainer || isCommitContainer) {
                // Déterminer le nombre de squelettes à créer
                const count = isReleaseContainer ? 2 : 3;
                const container = document.createElement('div');
                container.className = 'skeleton-container';
                
                // Créer les squelettes de cartes
                for (let i = 0; i < count; i++) {
                    const skeletonCard = document.createElement('div');
                    skeletonCard.className = 'skeleton-card';
                    skeletonCard.innerHTML = `
                        <div class="skeleton-title"></div>
                        <div class="skeleton-text"></div>
                        <div class="skeleton-text"></div>
                        <div class="skeleton-text"></div>
                    `;
                    container.appendChild(skeletonCard);
                }
                
                // Remplacer l'animation de chargement par les squelettes
                element.parentNode.replaceChild(container, element);
            }
        });
    }
    
    // Appliquer le remplacement après un court délai
    setTimeout(replaceLoadingAnimations, 100);
    
    // 4. Animations à l'apparition des éléments (Intersection Observer)
    const animateOnScroll = function() {
        const observer = new IntersectionObserver((entries) => {
            entries.forEach(entry => {
                if (entry.isIntersecting) {
                    entry.target.classList.add('visible');
                    observer.unobserve(entry.target);
                }
            });
        }, {
            threshold: 0.1
        });
        
        // Observer les éléments avec la classe .animate-on-scroll
        document.querySelectorAll('.animate-on-scroll').forEach(el => {
            observer.observe(el);
            // Ajouter une classe pour le style initial (invisible)
            el.classList.add('to-animate');
        });
    };
    
    // Initialiser l'animation au défilement
    animateOnScroll();
    
    // 5. Améliorer les boutons avec effet de clic
    document.querySelectorAll('button, .btn-primary, .btn-secondary, .download-button').forEach(button => {
        button.addEventListener('mousedown', function() {
            this.style.transform = 'scale(0.98)';
        });
        
        button.addEventListener('mouseup', function() {
            this.style.transform = '';
        });
        
        button.addEventListener('mouseleave', function() {
            this.style.transform = '';
        });
    });
    
    // 6. Ajouter la classe modern-card aux cartes existantes
    document.querySelectorAll('.release-card, .commit-card, .news-card').forEach(card => {
        card.classList.add('modern-card');
    });
    
    // 7. Ajouter des classes d'animation aux sections
    document.querySelectorAll('section').forEach((section, index) => {
        section.classList.add('fade-in');
        section.style.animationDelay = `${index * 0.1}s`;
        
        // Rendre les enfants directs séquentiels
        if (section.children.length > 1) {
            const contentContainer = section.querySelector('div');
            if (contentContainer) contentContainer.classList.add('stagger-children');
        }
    });
    
    // Exemple d'utilisation de la notification
    // Commenter cette ligne en production
    // setTimeout(() => showNotification('Bienvenue sur le site ICPocket !', 'info'), 1000);
    
    // Marquer le lien actif dans la navigation
    const currentPage = window.location.pathname.split('/').pop();
    const navLinks = document.querySelectorAll('nav button a');
    
    navLinks.forEach(link => {
        const linkHref = link.getAttribute('href');
        if (linkHref === currentPage || 
            (currentPage === '' && linkHref === 'index.html')) {
            link.classList.add('active');
            link.parentElement.classList.add('current-page');
        }
    });
    
    // Ajouter la navigation par "retour en haut" pour les longues pages
    const scrollToTopButton = document.createElement('button');
    scrollToTopButton.className = 'scroll-top-button';
    scrollToTopButton.innerHTML = '<i class="fas fa-arrow-up"></i>';
    scrollToTopButton.setAttribute('aria-label', 'Retour en haut de page');
    document.body.appendChild(scrollToTopButton);
    
    scrollToTopButton.addEventListener('click', () => {
        window.scrollTo({
            top: 0,
            behavior: 'smooth'
        });
    });
    
    // Afficher/masquer le bouton selon la position de défilement
    window.addEventListener('scroll', () => {
        if (window.scrollY > 300) {
            scrollToTopButton.classList.add('visible');
        } else {
            scrollToTopButton.classList.remove('visible');
        }
    });
    
    // Ajouter des transitions de page
    document.addEventListener('click', e => {
        const link = e.target.closest('a');
        
        if (link && 
            !link.hasAttribute('target') && 
            link.hostname === window.location.hostname && 
            link.getAttribute('href').indexOf('#') !== 0 && 
            !link.hasAttribute('download')) {
            
            e.preventDefault();
            const targetHref = link.getAttribute('href');
            
            document.body.classList.add('page-transition-out');
            
            setTimeout(() => {
                window.location.href = targetHref;
            }, 300);
        }
    });
    
    // Ajout: Traitement des événements d'erreur pour les images
    document.querySelectorAll('img').forEach(img => {
        img.addEventListener('error', function() {
            this.src = 'img/image-placeholder.jpg';
            this.classList.add('img-error');
        });
    });
    
    // Ajout: Chargement paresseux des images
    if ('IntersectionObserver' in window) {
        const imageObserver = new IntersectionObserver((entries) => {
            entries.forEach(entry => {
                if (entry.isIntersecting) {
                    const img = entry.target;
                    if (img.dataset.src) {
                        img.src = img.dataset.src;
                        img.removeAttribute('data-src');
                        imageObserver.unobserve(img);
                    }
                }
            });
        });
        
        document.querySelectorAll('img[data-src]').forEach(img => {
            imageObserver.observe(img);
        });
    }
    
    // Ajout: Notifications de dernière minute (à montrer une seule fois par session)
    if (!sessionStorage.getItem('notificationShown')) {
        setTimeout(() => {
            showNotification('Bienvenue sur le site ICPocket ! Découvrez notre dernière version.', 'info', 8000);
            sessionStorage.setItem('notificationShown', 'true');
        }, 2000);
    }
    
    // Initialisation du mode A/B testing
    initABTesting();
    
    // Initialiser le menu mobile
    initMobileMenu();
    
    // Ajout de classes pour les conteneurs flexibles
    document.querySelectorAll('.content-section').forEach(section => {
        section.classList.add('responsive-container');
    });
    
    // Gérer l'affichage de la sidebar sur mobile
    initSidebarToggle();
    
    // Initialiser le diaporama d'images pour mobile
    initMobileScreenshots();
});

// Ajout: Fonction pour A/B testing
function initABTesting() {
    // Exemple: tester deux variantes de bouton de téléchargement
    if (document.getElementById('downloadButton')) {
        // Générer aléatoirement la variante A ou B
        const downloadBtn = document.getElementById('downloadButton');
        downloadBtn.classList.add('download-button');
        downloadBtn.innerHTML = '<i class="fas fa-rocket"></i><span>Télécharger v0.9</span>';
    }
}

// Gestion du menu mobile
function initMobileMenu() {
    const menuToggle = document.getElementById('mobile-menu-toggle');
    const mainNav = document.getElementById('main-nav');
    
    if (menuToggle && mainNav) {
        // Bouton pour ouvrir/fermer le menu
        menuToggle.addEventListener('click', function() {
            if (mainNav) {
                mainNav.classList.toggle('mobile-active');
                
                // Changer l'icône selon l'état du menu
                const icon = this.querySelector('i');
                if (icon) {
                    if (mainNav.classList.contains('mobile-active')) {
                        icon.className = 'fas fa-times';
                        document.body.style.overflow = 'hidden'; // Bloquer le défilement
                    } else {
                        icon.className = 'fas fa-bars';
                        document.body.style.overflow = '';
                    }
                }
            }
        });
        
        // Fermer le menu quand on clique sur un lien
        mainNav.querySelectorAll('a').forEach(link => {
            link.addEventListener('click', function() {
                if (mainNav) {
                    mainNav.classList.remove('mobile-active');
                    mainNav.style.display = 'flex';
                    mainNav.style.justifyContent = 'center';
                    mainNav.style.alignItems = 'center';
                    menuToggle.querySelector('i').className = 'fas fa-bars';
                    document.body.style.overflow = '';
                }
            });
        });
    }
}

// Gestion du redimensionnement pour les changements d'orientation
window.addEventListener('resize', function() {
    // Vérifier si on est passé en mode desktop
    if (window.innerWidth >= 768) {
        // Rétablir l'affichage normal des screenshots
        const screenshotItems = document.querySelectorAll('.screenshot-item');
        const indicators = document.querySelector('.screenshot-indicators');
        const navButtons = document.querySelectorAll('.screenshot-nav');
        
        if (screenshotItems) {
            screenshotItems.forEach(item => {
                item.classList.remove('active');
                item.style.display = 'flex';
                item.style.opacity = '1';
                item.style.position = 'static';
            });
        }
        
        // Supprimer les éléments de navigation mobile
        if (indicators) indicators.remove();
        if (navButtons) navButtons.forEach(btn => btn.remove());
        
        // Rétablir l'affichage normal pour le conteneur
        const screenshotsContainer = document.querySelector('.screenshots');
        if (screenshotsContainer) {
            screenshotsContainer.style.height = 'auto';
            screenshotsContainer.style.position = 'static';
        }
    }
});

// Gérer l'affichage de la sidebar sur mobile
function initSidebarToggle() {
    const sidebarToggle = document.getElementById('sidebar-toggle');
    const sidebar = document.querySelector('.sidebar');
    
    if (sidebarToggle && sidebar) {
        // Créer le backdrop s'il n'existe pas
        let backdrop = document.querySelector('.sidebar-backdrop');
        if (!backdrop) {
            backdrop = document.createElement('div');
            backdrop.className = 'sidebar-backdrop';
            document.body.appendChild(backdrop);
        }
        
        // Fonction pour toggler la sidebar
        const toggleSidebar = () => {
            if (sidebar) {
                sidebar.classList.toggle('mobile-active');
                backdrop.classList.toggle('active');
                document.body.style.overflow = sidebar.classList.contains('mobile-active') ? 'hidden' : '';
            }
        };
        
        // Event listeners
        sidebarToggle.addEventListener('click', toggleSidebar);
        backdrop.addEventListener('click', toggleSidebar);
    }
}

// Initialiser le diaporama d'images pour mobile
function initMobileScreenshots() {
    // Ne pas initialiser si notre autre gestionnaire est déjà activé
    if (document.querySelector('.screenshots .screenshot-indicators')) return;
    
    // Exécuter seulement sur mobile
    if (window.innerWidth >= 768) return;
    
    const screenshotItems = document.querySelectorAll('.screenshot-item');
    
    // S'il n'y a pas d'images, sortir
    if (!screenshotItems.length) return;
    
    // Masquer tous les screenshots sauf le premier (preview)
    screenshotItems.forEach((item, index) => {
      if (index === 0) {
        item.classList.add('active');
        item.style.display = 'block';
      } else {
        item.classList.remove('active');
        item.style.display = 'none';
      }
    });
    
    // Ajouter les indicateurs (points) seulement s'ils n'existent pas déjà
    const screenshotsContainer = document.querySelector('.screenshots');
    if (screenshotsContainer && !screenshotsContainer.querySelector('.screenshot-indicators')) {
        // Créer le conteneur d'indicateurs
        const indicators = document.createElement('div');
        indicators.className = 'screenshot-indicators';
        
        // Ajouter un point pour chaque image
        screenshotItems.forEach((_, index) => {
            const dot = document.createElement('div');
            dot.className = index === 0 ? 'screenshot-dot active' : 'screenshot-dot';
            indicators.appendChild(dot);
        });
        
        // Ajouter les indicateurs au DOM
        screenshotsContainer.appendChild(indicators);
        
        // Ajouter l'attribut data-count pour afficher le nombre d'images
        screenshotsContainer.setAttribute('data-count', `1/${screenshotItems.length}`);
    }
    
    // Supprimer les écouteurs de clic existants pour éviter les doublons
    screenshotItems.forEach(item => {
        const newItem = item.cloneNode(true);
        item.parentNode.replaceChild(newItem, item);
    });
    
    // Récupérer les nouveaux éléments sans écouteurs
    const refreshedItems = document.querySelectorAll('.screenshot-item');
    
    // Ajouter l'écouteur de clic sur chaque image
    refreshedItems.forEach(item => {
        item.addEventListener('click', function(e) {
            // Obtenir les coordonnées relatives du clic
            const rect = this.getBoundingClientRect();
            const clickX = e.clientX - rect.left;
            
            // Si le clic est sur la moitié droite de l'image, passer à la suivante
            // Sinon, revenir à la précédente
            if (clickX > rect.width / 2) {
                // Trouver l'index de l'élément actif
                let currentIndex = Array.from(refreshedItems).findIndex(item => 
                    item.classList.contains('active')
                );
                
                // Déterminer le prochain index
                let nextIndex = (currentIndex + 1) % refreshedItems.length;
                
                // Mettre à jour les classes
                refreshedItems[currentIndex].classList.remove('active');
                refreshedItems[currentIndex].style.display = 'none';
                refreshedItems[nextIndex].classList.add('active');
                refreshedItems[nextIndex].style.display = 'block';
                
                // Mettre à jour les indicateurs
                const dots = document.querySelectorAll('.screenshot-dot');
                if (dots.length) {
                    dots[currentIndex].classList.remove('active');
                    dots[nextIndex].classList.add('active');
                }
                
                // Mettre à jour le compteur
                const container = document.querySelector('.screenshots');
                if (container) {
                    container.setAttribute('data-count', `${nextIndex + 1}/${refreshedItems.length}`);
                }
            } else {
                // Trouver l'index de l'élément actif
                let currentIndex = Array.from(refreshedItems).findIndex(item => 
                    item.classList.contains('active')
                );
                
                // Déterminer l'index précédent
                let prevIndex = (currentIndex - 1 + refreshedItems.length) % refreshedItems.length;
                
                // Mettre à jour les classes
                refreshedItems[currentIndex].classList.remove('active');
                refreshedItems[currentIndex].style.display = 'none';
                refreshedItems[prevIndex].classList.add('active');
                refreshedItems[prevIndex].style.display = 'block';
                
                // Mettre à jour les indicateurs
                const dots = document.querySelectorAll('.screenshot-dot');
                if (dots.length) {
                    dots[currentIndex].classList.remove('active');
                    dots[prevIndex].classList.add('active');
                }
                
                // Mettre à jour le compteur
                const container = document.querySelector('.screenshots');
                if (container) {
                    container.setAttribute('data-count', `${prevIndex + 1}/${refreshedItems.length}`);
                }
            }
        });
    });
}

// S'exécute uniquement sur les pages avec galerie
if (document.querySelector('.screenshots')) {
    // Code de gestion de la galerie
}

// S'exécute uniquement sur les pages avec menu mobile
if (document.querySelector('#mobile-menu')) {
    // Code de gestion du menu mobile
} 