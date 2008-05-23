#ifndef _RES_PUSHER_H_
#define _RES_PUSHER_H_


class Rigidbody;
typedef Rigidbody pusher_obj_t;

/**
 * Spring simulator.
 * This structures holds the necessary physics information
 * to simulate the spring physics between two particles.
 */
class Pusher
{
private:
    //! Spring's two particles
    pusher_obj_t *m_objects[2];

    //! Spring coefficient
    float m_Ks;

    //! Damping factor
    float m_Kd;

    //! Spring's rest length
    float m_restLength;

public:
    Pusher();
    Pusher( pusher_obj_t*, pusher_obj_t* );
    
    // Accessors
    float getKs() const { return m_Ks; }
    float getKd() const { return m_Kd; }
    float getRestLength() const { return m_restLength; }
    const pusher_obj_t* getFirstObject() const { return m_objects[0]; }
    const pusher_obj_t* getSecondObject() const { return m_objects[1]; }

    // Mutators
    void setObjects( pusher_obj_t *a, pusher_obj_t *b ) { 
        m_objects[0] = a; m_objects[1] = b; 
    }
    void setKs( float k ) { m_Ks = k; }
    void setKd( float d ) { m_Kd = d; }
    void setRestLength( float l ) { m_restLength = l; }
    
    //! Computes the forces acting on the particles
    void compute();
};

#endif
